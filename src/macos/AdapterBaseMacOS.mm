#import "AdapterBaseMacOS.h"

@implementation AdapterBaseMacOS

- (id)init {
    self = [super init];
    if (self) {
        NSLog(@"Constructor worked!\n");
    }
    return self;
}

- (void)scanStart {
    NSLog(@"Scan Start!\n");
}

- (void)scanStop {
    NSLog(@"Scan Stop!\n");
}

@end