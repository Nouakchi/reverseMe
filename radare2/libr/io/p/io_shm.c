/* radare - LGPL - Copyright 2008-2024 pancake */

#include "r_io.h"
#include "r_lib.h"
#include <sys/types.h>

#if __ANDROID__ || EMSCRIPTEN
#undef R2__UNIX__
#define R2__UNIX__ 0
#endif

// linux requires -lrt for this, but still it seems to not work as expected
// better not to enable it by default until we get enough time to properly
// make this work across all unixes without adding extra depenencies
#define USE_SHM_OPEN 0

#if R2__UNIX__ && !defined (__QNX__) && !defined (__HAIKU__)
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

typedef struct {
	int fd;
	int id;
	ut8 *buf;
	ut32 size;
} RIOShm;
#define RIOSHM_FD(x) (((RIOShm*)(x))->fd)

#define SHMATSZ 0x9000; // 32*1024*1024; /* 32MB : XXX not used correctly? */

static int shm__write(RIO *io, RIODesc *fd, const ut8 *buf, int count) {
	R_RETURN_VAL_IF_FAIL (fd && fd->data, -1);
	RIOShm *shm = fd->data;
	if (shm->buf) {
		(void)memcpy (shm->buf + io->off, buf, count);
		return count;
	}
	return write (shm->fd, buf, count);
}

static int shm__read(RIO *io, RIODesc *fd, ut8 *buf, int count) {
	R_RETURN_VAL_IF_FAIL (fd && fd->data, -1);
	RIOShm *shm = fd->data;
	if (io->off + count >= shm->size) {
		if (io->off > shm->size) {
			return -1;
		}
		count = shm->size - io->off;
	}
	if (shm->buf) {
		memcpy (buf, shm->buf + io->off , count);
		return count;
	}
	return read (shm->fd, buf, count);
}

static bool shm__close(RIODesc *fd) {
	R_RETURN_VAL_IF_FAIL (fd && fd->data, -1);
	RIOShm *shm = fd->data;
	int ret = (shm->buf)
		? shmdt (((RIOShm*)(fd->data))->buf)
		: close (shm->fd);
	R_FREE (fd->data);
	return ret == 0;
}

static ut64 shm__lseek(RIO *io, RIODesc *fd, ut64 offset, int whence) {
	R_RETURN_VAL_IF_FAIL (fd && fd->data, -1);
	RIOShm *shm = fd->data;
	switch (whence) {
	case R_IO_SEEK_SET:
		io->off = offset;
		break;
	case R_IO_SEEK_CUR:
		if (io->off + offset > shm->size) {
			io->off = shm->size;			//XXX
		} else {
			io->off += offset;			//XXX
		}
		break;
	case R_IO_SEEK_END:
		if ((int)shm->size > 0) {
			io->off = shm->size + (int)offset;	//XXX
		} else {
			// UT64_MAX means error
			io->off = UT64_MAX - 1;			//XXX
		}
		break;
	}
	return io->off;
}

static bool shm__plugin_open(RIO *io, const char *pathname, bool many) {
	return r_str_startswith (pathname, "shm://");
}

static inline int getshmfd(RIOShm *shm) {
	return (((int)(size_t)shm->buf) >> 4) & 0xfff;
}

static RIODesc *shm__open(RIO *io, const char *pathname, int rw, int mode) {
	if (r_str_startswith (pathname, "shm://")) {
		RIOShm *shm = R_NEW0 (RIOShm);
		if (!shm) {
			return NULL;
		}
		const char *ptr = pathname + 6;
		shm->id = atoi (ptr);
		if (!shm->id) {
			shm->id = r_str_hash (ptr);
		}
		shm->buf = shmat (shm->id, 0, 0);
		if (shm->buf == (void*)(size_t)-1) {
#if USE_SHM_OPEN
			shm->buf = NULL;
			shm->fd = shm_open (ptr, O_CREAT | (rw?O_RDWR:O_RDONLY), 0644);
#else
			shm->fd = -1;
#endif

		} else {
			shm->fd = getshmfd (shm);
		}
		shm->size = SHMATSZ;
		if (shm->fd != -1) {
			R_LOG_INFO ("Connected to shared memory 0x%08x", shm->id);
			return r_io_desc_new (io, &r_io_plugin_shm, pathname, rw, mode, shm);
		}
		R_LOG_ERROR ("Cannot connect to shared memory (%d)", shm->id);
		free (shm);
	}
	return NULL;
}

RIOPlugin r_io_plugin_shm = {
	.meta = {
		.name = "shm",
		.desc = "Shared memory resources plugin",
		.license = "MIT",
		.author = "pancake",
	},
	.uris = "shm://",
	.open = shm__open,
	.close = shm__close,
	.read = shm__read,
	.check = shm__plugin_open,
	.seek = shm__lseek,
	.write = shm__write,
};

#else
RIOPlugin r_io_plugin_shm = {
	.meta = {
		.name = "shm",
		.desc = "shared memory resources (not for this platform)",
	}
};
#endif

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_IO,
	.data = &r_io_plugin_shm,
	.version = R2_VERSION
};
#endif
