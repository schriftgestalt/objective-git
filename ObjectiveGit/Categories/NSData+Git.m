//
//  NSData+Git.m
//

#import "NSData+Git.h"
#import "NSError+Git.h"

#import "git2/errors.h"

extern int git_buf_is_binary(const git_buf *str);

@implementation NSData (Git)

+ (NSData *)git_dataWithOid:(git_oid *)oid {
	return [NSData dataWithBytes:oid length:sizeof(git_oid)];
}

- (BOOL)git_getOid:(git_oid *)oid error:(NSError *__autoreleasing *)error {
	if ([self length] != sizeof(git_oid)) {
		if (error != NULL) {
			*error = [NSError errorWithDomain:GTGitErrorDomain 
										 code:GIT_ERROR_INVALID
									 userInfo:@{
				NSLocalizedDescriptionKey: @"can't extract oid from data of incorrect length"
			}];
		}
		return NO;
	}
	
	[self getBytes:oid length:sizeof(git_oid)];
	return YES;
}

+ (instancetype)git_dataWithBuffer:(git_buf *)buffer {
	NSCParameterAssert(buffer != NULL);

	if (buffer->size == 0) return [self data];

#if XXX
	// Ensure that the buffer is actually allocated dynamically, not pointing to
	// some data which may disappear.
	if (git_buf_grow(buffer, 0) != GIT_OK) return nil;
#endif

	NSData *data = [self dataWithBytesNoCopy:buffer->ptr length:buffer->size freeWhenDone:YES];
	*buffer = (git_buf){NULL};

	return data;
}

- (git_buf)git_buf {
	return (git_buf){(void *)self.bytes, self.length};
}

- (BOOL)git_containsNUL {
	return memchr((void *)self.bytes, '\0', self.length);
}

- (BOOL)git_isBinary {
	git_buf buffer = self.git_buf;
	return git_buf_is_binary(&buffer) > 0;
}

@end
