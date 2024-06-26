//
//  GTRepository+Committing.h
//  ObjectiveGitFramework
//
//  Created by Josh Abernathy on 9/30/13.
//  Copyright (c) 2013 GitHub, Inc. All rights reserved.
//

#import "GTRepository.h"

NS_ASSUME_NONNULL_BEGIN

@interface GTRepository (Committing)

/// Creates a new commit.
///
/// tree      - The tree used for the commit. Cannot be nil.
/// message   - The commit message. Cannot be nil.
/// author    - The author of the commit. Cannot be nil.
/// committer - The committer of the commit. Cannot be nil.
/// parents   - An array of GTCommits. May be nil, which means the commit has no
///             parents.
/// refName   - The ref name which will be updated to point at the new commit.
///             May be nil.
/// error     - The error if one occurred.
///
/// Returns the newly created commit, or nil if an error occurred.
- (GTCommit * _Nullable)createCommitWithTree:(GTTree *)tree message:(NSString *)message author:(GTSignature *)author committer:(GTSignature *)committer parents:(NSArray<GTCommit *> * _Nullable)parents updatingReferenceNamed:(NSString * _Nullable)refName error:(NSError *__autoreleasing *)error;

/// Creates a new commit using +createCommitWithTree:message:author:committer:parents:updatingReferenceNamed:error:
/// with -userSignatureForNow as both the author and committer.
- (GTCommit * _Nullable)createCommitWithTree:(GTTree *)tree message:(NSString *)message parents:(NSArray<GTCommit *> * _Nullable)parents updatingReferenceNamed:(NSString * _Nullable)refName error:(NSError *__autoreleasing *)error;

@end

NS_ASSUME_NONNULL_END
