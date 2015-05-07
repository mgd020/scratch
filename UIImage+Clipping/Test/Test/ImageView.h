//
//  ImageView.h
//  Test
//
//  Created by Matthew Downey on 25/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ImageView : UIView
{
    UIImage *_image;
}

@property (nonatomic, retain) UIImage *image;

@end
