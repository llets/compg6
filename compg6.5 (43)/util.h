#ifndef UTIL_H
#define	UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <assert.h>
#include "types.h"

using namespace std;

bool ReadFile(const char* fileName, string& outFile);

void OgldevError(const char* pFileName, uint line, const char* pError);
void OgldevFileError(const char* pFileName, uint line, const char* pFileError);

#define OGLDEV_ERROR(Error) OgldevError(__FILE__, __LINE__, Error);
#define OGLDEV_FILE_ERROR(FileError) OgldevFileError(__FILE__, __LINE__, FileError);

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define SNPRINTF snprintf
#define RANDOM random
#define SRANDOM srand(time(0))

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define GLExitIfError                                                           \
{                                                                               \
    GLenum Error = glGetError();                                                \
                                                                                \
    if (Error != GL_NO_ERROR) {                                                 \
        printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);     \
        exit(0);                                                                \
    }                                                                           \
}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

long long GetCurrentTimeMillis();

#endif