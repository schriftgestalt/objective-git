//
//  GTRepository+Stashing.h
//  ObjectiveGitFramework
//
//  Created by Justin Spahr-Summers on 2013-09-27.
//  Copyright (c) 2013 GitHub, Inc. All rights reserved.
//

#import "GTRepository.h"

#import "git2/stash.h"

/// Flags for -stashChangesWithMessage:flags:error:.
/// Those can be ORed together. See git_stash_flags for additional information.
typedef NS_OPTIONS(uint32_t, GTRepositoryStashFlag) {
	GTRepositoryStashFlagDefault = GIT_STASH_DEFAULT,
	GTRepositoryStashFlagKeepIndex = GIT_STASH_KEEP_INDEX,
	GTRepositoryStashFlagIncludeUntracked = GIT_STASH_INCLUDE_UNTRACKED,
	GTRepositoryStashFlagIncludeIgnored = GIT_STASH_INCLUDE_IGNORED
};

/// Flags for -applyStashAtIndex:flags:error: and
/// -popStashAtIndex:flags:error.
/// Those can be ORed together. See git_stash_apply_flags for additional information.
typedef NS_OPTIONS(uint32_t, GTRepositoryStashApplyFlag) {
	GTRepositoryStashApplyFlagDefault = GIT_STASH_APPLY_DEFAULT,
	GTRepositoryStashApplyFlagReinstateIndex = GIT_STASH_APPLY_REINSTATE_INDEX,
};

/// Enum representing the current state of a stash apply/pop operation.
/// See git_stash_apply_progress_t for additional information.
typedef NS_ENUM(NSInteger, GTRepositoryStashApplyProgress) {
	GTRepositoryStashApplyProgressNone = GIT_STASH_APPLY_PROGRESS_NONE,
	GTRepositoryStashApplyProgressLoadingStash = GIT_STASH_APPLY_PROGRESS_LOADING_STASH,
	GTRepositoryStashApplyProgressAnalyzeIndex = GIT_STASH_APPLY_PROGRESS_ANALYZE_INDEX,
	GTRepositoryStashApplyProgressAnalyzeModified = GIT_STASH_APPLY_PROGRESS_ANALYZE_MODIFIED,
	GTRepositoryStashApplyProgressAnalyzeUntracked = GIT_STASH_APPLY_PROGRESS_ANALYZE_UNTRACKED,
	GTRepositoryStashApplyProgressCheckoutUntracked = GIT_STASH_APPLY_PROGRESS_CHECKOUT_UNTRACKED,
	GTRepositoryStashApplyProgressCheckoutModified = GIT_STASH_APPLY_PROGRESS_CHECKOUT_MODIFIED,
	GTRepositoryStashApplyProgressDone = GIT_STASH_APPLY_PROGRESS_DONE,
};

NS_ASSUME_NONNULL_BEGIN

@interface GTRepository (Stashing)

/// Stash the repository's changes.
///
/// message   - Message to be attributed to the item in the stash. This may be
///             nil.
/// stashFlag - The flags of stash to be used.
/// error     - If not NULL, set to any error that occurred.
///
/// Returns a commit representing the stashed changes if successful, or nil
/// otherwise.
- (GTCommit * _Nullable)stashChangesWithMessage:(NSString * _Nullable)message flags:(GTRepositoryStashFlag)flags error:(NSError *__autoreleasing *)error;

/// Enumerate over all the stashes in the repository, from most recent to oldest.
///
/// block - A block to execute for each stash found. `index` will be the zero-based
///         stash index (where 0 is the most recent stash). Setting `stop` to YES
///         will cause enumeration to stop after the block returns. Must not be nil.
- (void)enumerateStashesUsingBlock:(void (^)(NSUInteger index, NSString * _Nullable message, GTOID * _Nullable oid, BOOL *stop))block;

/// Apply stashed changes.
///
/// index         - The index of the stash to apply. 0 is the latest one.
/// flags         - The flags to use when applying the stash.
/// options       - The options to use when checking out (if nil, use the defaults provided by libgit2).
/// error         - If not NULL, set to any error that occurred.
/// progressBlock - A block that will be executed on each step of the stash application.
///
/// Returns YES if the requested stash was successfully applied, NO otherwise.
- (BOOL)applyStashAtIndex:(NSUInteger)index flags:(GTRepositoryStashApplyFlag)flags checkoutOptions:(GTCheckoutOptions * _Nullable)options error:(NSError *__autoreleasing *)error progressBlock:(void (^ _Nullable)(GTRepositoryStashApplyProgress progress, BOOL *stop))progressBlock;

/// Pop stashed changes.
///
/// index         - The index of the stash to apply. 0 is the most recent stash.
/// flags         - The flags to use when applying the stash.
/// options       - The options to use when checking out (if nil, use the defaults provided by libgit2).
/// error         - If not NULL, set to any error that occurred.
/// progressBlock - A block that will be executed on each step of the stash application.
///
/// Returns YES if the requested stash was successfully applied, NO otherwise.
- (BOOL)popStashAtIndex:(NSUInteger)index flags:(GTRepositoryStashApplyFlag)flags checkoutOptions:(GTCheckoutOptions * _Nullable)options error:(NSError *__autoreleasing *)error progressBlock:(void (^ _Nullable)(GTRepositoryStashApplyProgress progress, BOOL *stop))progressBlock;

/// Drop a stash from the repository's list of stashes.
///
/// index - The index of the stash to drop, where 0 is the most recent stash.
/// error - If not NULL, set to any error that occurs.
///
/// Returns YES if the stash was successfully dropped, NO otherwise
- (BOOL)dropStashAtIndex:(NSUInteger)index error:(NSError *__autoreleasing *)error;

@end

NS_ASSUME_NONNULL_END
