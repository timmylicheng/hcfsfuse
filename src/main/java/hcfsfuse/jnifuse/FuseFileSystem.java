package hcfsfuse.jnifuse;

import alluxio.jnifuse.FuseFillDir;
import hcfsfuse.jnifuse.struct.FileStat;
import hcfsfuse.jnifuse.struct.FuseContext;
import hcfsfuse.jnifuse.struct.FuseFileInfo;
import hcfsfuse.jnifuse.struct.Statvfs;

import java.nio.ByteBuffer;
import java.util.concurrent.ThreadLocalRandom;

public interface FuseFileSystem {

  default int getattr(String path, FileStat stat) {
    throw new UnsupportedOperationException("getattr");
  }

  default int mkdir(String path, long mode) {
    throw new UnsupportedOperationException("mkdir");
  }

  default int unlink(String path) {
    throw new UnsupportedOperationException("unlink");
  }

  default int rmdir(String path) {
    throw new UnsupportedOperationException("rmdir");
  }

  default int symlink(String oldpath, String newpath) {
    throw new UnsupportedOperationException("symlink");
  }

  default int rename(String oldpath, String newpath) {
    throw new UnsupportedOperationException("rename");
  }

  default int link(String oldpath, String newpath) {
    throw new UnsupportedOperationException("link");
  }

  default int chmod(String path, long mode) {
    throw new UnsupportedOperationException("chmod");
  }

  default int chown(String path, long uid, long gid) {
    throw new UnsupportedOperationException("chown");
  }

  default int truncate(String path, long size) {
    throw new UnsupportedOperationException("truncate");
  }

  default int open(String path, FuseFileInfo fi) {
    throw new UnsupportedOperationException("open");
  }

  default int read(String path, ByteBuffer buf, long size, long offset, FuseFileInfo fi) {
    throw new UnsupportedOperationException("read");
  }

  default int write(String path, ByteBuffer buf, long size, long offset, FuseFileInfo fi) {
    throw new UnsupportedOperationException("write");
  }

  default int statfs(String path, Statvfs stbuf) {
    throw new UnsupportedOperationException("statfs");
  }

  default int flush(String path, FuseFileInfo fi) {
    throw new UnsupportedOperationException("flush");
  }

  default int release(String path, FuseFileInfo fi) {
    throw new UnsupportedOperationException("release");
  }

  default int opendir(String path, FuseFileInfo fi) {
    throw new UnsupportedOperationException("opendir");
  }

  default int readdir(String path, long bufaddr, FuseFillDir filter, long offset, FuseFileInfo fi) {
    throw new UnsupportedOperationException("readdir");
  }

  default int releasedir(String path, FuseFileInfo fi) {
    throw new UnsupportedOperationException("releasedir");
  }

  default int create(String path, long mode, FuseFileInfo fi) {
    throw new UnsupportedOperationException("create");
  }

  default FuseContext getContext() {
    return new FuseContext(ByteBuffer.allocate(32));
  }

  default String getFileSystemName() {
    return "fusefs" + ThreadLocalRandom.current().nextInt();
  }
}
