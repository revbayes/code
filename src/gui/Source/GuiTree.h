#import <Cocoa/Cocoa.h>
@class Node;



@interface GuiTree : NSObject <NSCoding> {

	NSMutableArray*   nodes;
    NSMutableArray*   downPassSequence;
    BOOL              initializedDownPass;
    Node*             root;
    int               numberOfTaxa;
	NSString*         info;
}

@property (nonatomic)        BOOL      initializedDownPass;
@property (nonatomic,strong) NSString* info;
@property (nonatomic,strong) Node*     root;

- (void)addTaxonToRandomBranch;
- (void)buildRandomTreeWithSize:(int)n;
- (void)deselectAllNodes;
- (Node*)downPassNodeIndexed:(int)idx;
- (Node*)getRoot;
- (id)initWithTipSize:(int)n;
- (void)initializeDownPassSequence;
- (BOOL)isRoot:(Node*)p;
- (void)moveFromBranch:(Node*)fromBranch toBranch:(Node*)toBranch forTreeYCoordinates:(float)c;
- (Node*)nodeWithName:(NSString*)str;
- (int)numberOfNodes;
- (int)getNumberOfTaxa;
- (void)passDown:(Node*)p;
- (void)print;
- (void)removeSubtreeAboveNode:(Node*)p;
- (void)rootTreeOnNode:(Node*)p;
- (void)setCoordinates:(BOOL)drawMonophyleticWrOutgroup;
- (void)setAllFlagsTo:(BOOL)tf;
- (void)setXCoordinates;
- (void)setNodesToArray:(NSMutableArray*)n;

@end
