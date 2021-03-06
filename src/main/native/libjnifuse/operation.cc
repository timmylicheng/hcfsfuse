#include "operation.h"

#include "debug.h"

namespace jnifuse {

Operation::Operation() {
  this->fs = nullptr;
  this->obj = nullptr;
  this->clazz = nullptr;
  this->signature = nullptr;
  this->methodID = nullptr;
}

Operation::~Operation() {}

GetattrOperation::GetattrOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;Ljava/nio/ByteBuffer;)I";
  this->methodID = env->GetMethodID(this->clazz, "getattrCallback", signature);
}

int GetattrOperation::call(const char *path, struct stat *stbuf) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);
  jobject buffer = env->NewDirectByteBuffer((void *)stbuf, sizeof(struct stat));

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, buffer);

  env->DeleteLocalRef(jspath);
  env->DeleteLocalRef(buffer);

  return ret;
}

OpenOperation::OpenOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;Ljava/nio/ByteBuffer;)I";
  this->methodID = env->GetMethodID(this->clazz, "openCallback", signature);
}

int OpenOperation::call(const char *path, struct fuse_file_info *fi) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);
  jobject fibuf =
      env->NewDirectByteBuffer((void *)fi, sizeof(struct fuse_file_info));

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, fibuf);

  env->DeleteLocalRef(jspath);
  env->DeleteLocalRef(fibuf);

  return ret;
}

ReadOperation::ReadOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature =
      "(Ljava/lang/String;Ljava/nio/ByteBuffer;JJLjava/nio/ByteBuffer;)I";
  this->methodID = env->GetMethodID(this->clazz, "readCallback", signature);
}

int ReadOperation::call(const char *path, char *buf, size_t size, off_t offset,
                        struct fuse_file_info *fi) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);
  jobject buffer = env->NewDirectByteBuffer((void *)buf, size);
  jobject fibuf =
      env->NewDirectByteBuffer((void *)fi, sizeof(struct fuse_file_info));

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, buffer, size,
                               offset, fibuf);
  env->DeleteLocalRef(jspath);
  env->DeleteLocalRef(buffer);
  env->DeleteLocalRef(fibuf);

  return ret;
}

ReaddirOperation::ReaddirOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature =
      "(Ljava/lang/String;JLalluxio/jnifuse/FuseFillDir;JLjava/nio/"
      "ByteBuffer;)I";
  this->methodID = env->GetMethodID(this->clazz, "readdirCallback", signature);
  this->fillerclazz = env->FindClass("alluxio/jnifuse/FuseFillDir");
  this->fillerconstructor = env->GetMethodID(fillerclazz, "<init>", "(J)V");
}

int ReaddirOperation::call(const char *path, void *buf, fuse_fill_dir_t filler,
                           off_t offset, struct fuse_file_info *fi) {
  JNIEnv *env = this->fs->getEnv();
  jobject fillerobj = env->NewObject(this->fillerclazz, this->fillerconstructor,
                                     (void *)filler);
  jstring jspath = env->NewStringUTF(path);
  jobject fibuf =
      env->NewDirectByteBuffer((void *)fi, sizeof(struct fuse_file_info));

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, buf,
                               fillerobj, offset, fibuf);
  env->DeleteLocalRef(fillerobj);
  env->DeleteLocalRef(jspath);
  env->DeleteLocalRef(fibuf);

  return ret;
}

UnlinkOperation::UnlinkOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;)I";
  this->methodID = env->GetMethodID(this->clazz, "unlinkCallback", signature);
}

int UnlinkOperation::call(const char *path) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath);

  env->DeleteLocalRef(jspath);

  return ret;
}

FlushOperation::FlushOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;Ljava/nio/ByteBuffer;)I";
  this->methodID = env->GetMethodID(this->clazz, "flushCallback", signature);
}

int FlushOperation::call(const char *path, struct fuse_file_info *fi) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);
  jobject fibuf =
      env->NewDirectByteBuffer((void *)fi, sizeof(struct fuse_file_info));

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, fibuf);

  env->DeleteLocalRef(jspath);
  env->DeleteLocalRef(fibuf);

  return ret;
}

ReleaseOperation::ReleaseOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;Ljava/nio/ByteBuffer;)I";
  this->methodID = env->GetMethodID(this->clazz, "releaseCallback", signature);
}

int ReleaseOperation::call(const char *path, struct fuse_file_info *fi) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);
  jobject fibuf =
      env->NewDirectByteBuffer((void *)fi, sizeof(struct fuse_file_info));

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, fibuf);

  env->DeleteLocalRef(jspath);
  env->DeleteLocalRef(fibuf);

  return ret;
}

ChmodOperation::ChmodOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;J)I";
  this->methodID = env->GetMethodID(this->clazz, "chmodCallback", signature);
}

int ChmodOperation::call(const char *path, mode_t mode) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, mode);

  env->DeleteLocalRef(jspath);

  return ret;
}

// TODO: ChownOperation

CreateOperation::CreateOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;JLjava/nio/ByteBuffer;)I";
  this->methodID = env->GetMethodID(this->clazz, "createCallback", signature);
}

int CreateOperation::call(const char *path, mode_t mode,
                          struct fuse_file_info *fi) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);
  jobject fibuf =
      env->NewDirectByteBuffer((void *)fi, sizeof(struct fuse_file_info));

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, mode, fibuf);

  env->DeleteLocalRef(jspath);
  env->DeleteLocalRef(fibuf);

  return ret;
}

MkdirOperation::MkdirOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;J)I";
  this->methodID = env->GetMethodID(this->clazz, "mkdirCallback", signature);
}

int MkdirOperation::call(const char *path, mode_t mode) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, mode);

  env->DeleteLocalRef(jspath);

  return ret;
}

RmdirOperation::RmdirOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;)I";
  this->methodID = env->GetMethodID(this->clazz, "rmdirCallback", signature);
}

int RmdirOperation::call(const char *path) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath);

  env->DeleteLocalRef(jspath);

  return ret;
}

WriteOperation::WriteOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature =
      "(Ljava/lang/String;Ljava/nio/ByteBuffer;JJLjava/nio/ByteBuffer;)I";
  this->methodID = env->GetMethodID(this->clazz, "writeCallback", signature);
}

int WriteOperation::call(const char *path, const char *buf, size_t size,
                         off_t off, struct fuse_file_info *fi) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(path);
  jobject buffer = env->NewDirectByteBuffer((void *)buf, size);
  jobject fibuf =
      env->NewDirectByteBuffer((void *)fi, sizeof(struct fuse_file_info));

  int ret = env->CallIntMethod(this->obj, this->methodID, jspath, buffer, size,
                               off, fibuf);

  env->DeleteLocalRef(jspath);
  env->DeleteLocalRef(buffer);
  env->DeleteLocalRef(fibuf);

  return ret;
}

RenameOperation::RenameOperation(JniFuseFileSystem *fs) {
  this->fs = fs;
  JNIEnv *env = this->fs->getEnv();
  this->obj = this->fs->getFSObj();
  this->clazz = env->GetObjectClass(this->fs->getFSObj());
  this->signature = "(Ljava/lang/String;Ljava/lang/String;)I";
  this->methodID = env->GetMethodID(this->clazz, "renameCallback", signature);
}

int RenameOperation::call(const char *oldPath, const char *newPath) {
  JNIEnv *env = this->fs->getEnv();
  jstring jspath = env->NewStringUTF(oldPath);
  jstring jspathNew = env->NewStringUTF(newPath);

   int ret = env->CallIntMethod(this->obj, this->methodID, jspath, jspathNew);

  env->DeleteLocalRef(jspath);
  env->DeleteLocalRef(jspathNew);

  return ret;
}

}  // namespace jnifuse
