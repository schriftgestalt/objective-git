//
//  GTTreeEntry.m
//  ObjectiveGitFramework
//
//  Created by Timothy Clem on 2/22/11.
//
//  The MIT License
//
//  Copyright (c) 2011 Tim Clem
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#import "GTTreeEntry.h"
#import "GTObject.h"
#import "GTTree.h"
#import "GTRepository.h"
#import "NSError+Git.h"
#import "NSString+Git.h"
#import "GTOID.h"

#import "git2/errors.h"

@interface GTTreeEntry ()
@property (nonatomic, assign, readonly) git_tree_entry *git_tree_entry;
@end

@implementation GTTreeEntry

#pragma mark NSObject

- (NSString *)description {
	return [NSString stringWithFormat:@"<%@: %p> name: %@, type: %@, sha: %@, attributes: %lu", NSStringFromClass(self.class), self, self.name, self.typeString, self.SHA, (unsigned long)self.attributes];
}

- (NSUInteger)hash {
	return self.OID.hash;
}

- (BOOL)isEqual:(id)object {
	if ([object isKindOfClass:[self class]]) {
		return [self isEqualToEntry:object];
	}
	return [super isEqual:object];
}

- (BOOL)isEqualToEntry:(GTTreeEntry *)treeEntry {
	return git_tree_entry_cmp(self.git_tree_entry, treeEntry.git_tree_entry) == 0 ? YES : NO;
}

- (void)dealloc {
	git_tree_entry_free(_git_tree_entry);
}

#pragma mark API

- (instancetype)initWithEntry:(const git_tree_entry *)theEntry parentTree:(GTTree *)parent error:(NSError *__autoreleasing *)error {
	NSParameterAssert(theEntry != NULL);
	
	self = [super init];
	if (self == nil) return nil;
	
	git_tree_entry *copyOfEntry = nil;
	int gitError = git_tree_entry_dup(&copyOfEntry, theEntry);
	if (gitError != GIT_OK) {
		if (error != NULL) *error = [NSError git_errorFor:gitError description:@"Failed to duplicate tree entry."];
		return nil;
	}
	
	_git_tree_entry = copyOfEntry;
	_tree = parent;

	return self;
}

+ (instancetype)entryWithEntry:(const git_tree_entry *)theEntry parentTree:(GTTree *)parent error:(NSError *__autoreleasing *)error {
	return [[self alloc] initWithEntry:theEntry parentTree:parent error:error];
}

- (NSString *)name {
	NSString *name = @(git_tree_entry_name(self.git_tree_entry));
	NSAssert(name, @"name was nil");
	return name;
}

- (NSInteger)attributes {
	return git_tree_entry_filemode(self.git_tree_entry);
}

- (GTOID *)OID {
	return [GTOID oidWithGitOid:git_tree_entry_id(self.git_tree_entry)];
}

- (NSString *)SHA {
	return self.OID.SHA;
}

- (GTObjectType)type {
	return (GTObjectType)git_tree_entry_type(self.git_tree_entry);
}

- (NSString *)typeString {
	return @(git_object_type2string(git_tree_entry_type(self.git_tree_entry)));
}

- (GTRepository *)repository {
	return self.tree.repository;
}

- (GTObject *)GTObject:(NSError *__autoreleasing *)error {
	return [GTObject objectWithTreeEntry:self error:error];
}

@end


@implementation GTObject (GTTreeEntry)

+ (instancetype)objectWithTreeEntry:(GTTreeEntry *)treeEntry error:(NSError *__autoreleasing *)error {
	return [[self alloc] initWithTreeEntry:treeEntry error:error];
}

- (instancetype)initWithTreeEntry:(GTTreeEntry *)treeEntry error:(NSError *__autoreleasing *)error {
	git_object *obj;
	int gitError = git_tree_entry_to_object(&obj, treeEntry.repository.git_repository, treeEntry.git_tree_entry);
	if (gitError < GIT_OK) {
		if (error != NULL) {
			*error = [NSError git_errorFor:gitError description:@"Failed to get object for tree entry."];
		}
		return nil;
	}

	return [self initWithObj:obj inRepository:treeEntry.repository];
}

@end
