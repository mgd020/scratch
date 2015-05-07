#import "UIImage+Clipping.h"

@implementation UIImage (Clipping)

typedef struct
{
    CGImageRef image;
    CGRect rect;
} DrawAtPointInRectInfo;

void drawAtPointInRect(DrawAtPointInRectInfo *, CGContextRef);

- (CGRect)rectForDrawingInRect:(CGRect)rect fromRect:(CGRect)srcRect
{
    return CGRectMake(
            rect.origin.x - srcRect.origin.x, 
            rect.origin.y - srcRect.origin.y, 
            self.size.width * rect.size.width / srcRect.size.width, 
            self.size.height * rect.size.height / srcRect.size.height);
}

- (void)drawAtPoint:(CGPoint)point fromRect:(CGRect)srcRect
{
    [self drawInRect:CGRectMake(point.x, point.y, srcRect.size.width, srcRect.size.height) fromRect:srcRect];
}

- (void)drawAtPoint:(CGPoint)point fromRect:(CGRect)srcRect blendMode:(CGBlendMode)blendMode alpha:(CGFloat)alpha
{
    [self drawInRect:CGRectMake(point.x, point.y, srcRect.size.width, srcRect.size.height) fromRect:srcRect blendMode:blendMode alpha:alpha];
}

- (void)drawInRect:(CGRect)rect fromRect:(CGRect)srcRect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSaveGState(context);
    CGContextClipToRect(context, rect);
    [self drawInRect:[self rectForDrawingInRect:rect fromRect:srcRect]];
    CGContextRestoreGState(context);
}

- (void)drawInRect:(CGRect)rect fromRect:(CGRect)srcRect blendMode:(CGBlendMode)blendMode alpha:(CGFloat)alpha
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSaveGState(context);
    CGContextClipToRect(context, rect);
    [self drawInRect:[self rectForDrawingInRect:rect fromRect:srcRect] blendMode:blendMode alpha:alpha];
     CGContextRestoreGState(context);
}

- (void)drawAsPatternInRect:(CGRect)rect fromRect:(CGRect)srcRect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    // flip y coords
    srcRect.origin.y = self.size.height - srcRect.size.height - srcRect.origin.y;
    
    // save context
    CGContextSaveGState(context);
    
    // create pattern
    static const CGPatternCallbacks kDrawAsPatternInRectCallback = { 0, (CGPatternDrawPatternCallback)&drawAtPointInRect, NULL };
    CGRect bounds = CGRectMake(0, 0, srcRect.size.width, srcRect.size.height);
    DrawAtPointInRectInfo info;
    info.image = [self CGImage];
    info.rect = [self rectForDrawingInRect:bounds fromRect:srcRect];
    CGPatternRef pattern = CGPatternCreate(
            &info, // passed to drawAtPointInRect
            bounds, // bounding box of cell
            CGAffineTransformMake(1, 0, 0, -1, 0, srcRect.size.height), // transform to user space          
            bounds.size.width, // xstep
            bounds.size.height, // ystep
            kCGPatternTilingConstantSpacing, // tiling method
            true, // coloured
            &kDrawAsPatternInRectCallback); // draw callback
    
    // set pattern colour space
    CGColorSpaceRef patternSpace = CGColorSpaceCreatePattern(NULL);
    float alpha = 1.f;
    CGColorRef patternColor = CGColorCreateWithPattern(patternSpace, pattern, &alpha);
    CGColorSpaceRelease(patternSpace);
    CGPatternRelease(pattern);
    
    // draw pattern
    CGContextSetFillColorWithColor(context, patternColor);
    CGContextFillRect(context, rect);
    CGColorRelease(patternColor);
    
    // restore context
    CGContextRestoreGState(context);
}

void drawAtPointInRect(DrawAtPointInRectInfo *info, CGContextRef context)
{
    CGContextDrawImage(context, info->rect, info->image);
}

@end
