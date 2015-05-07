//
//  ImageView.m
//  Test
//
//  Created by Matthew Downey on 25/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "ImageView.h"
#import "UIImage+Clipping.h"

@implementation ImageView

@synthesize image=_image;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
//    [_image drawAsPatternInRect:self.bounds fromRect:CGRectMake(0,0,16,16)]; // red
//    [_image drawAsPatternInRect:self.bounds fromRect:CGRectMake(0,16,16,16)]; // green
//    [_image drawAsPatternInRect:self.bounds fromRect:CGRectMake(16,0,16,16)]; // blue
//    [_image drawAsPatternInRect:self.bounds fromRect:CGRectMake(16,16,16,16)]; // yellow
//    [_image drawAsPatternInRect:self.bounds fromRect:CGRectMake(32,32,16,16)]; // magenta
//    [_image drawAsPatternInRect:self.bounds fromRect:CGRectMake(16,16,32,32)];
//    [_image drawAtPoint:CGPointMake(20, 20) fromRect:CGRectMake(0, 0, 48, 48)];
//    [_image drawInRect:self.bounds fromRect:CGRectMake(0, 0, 15, 15)];
}

@end
