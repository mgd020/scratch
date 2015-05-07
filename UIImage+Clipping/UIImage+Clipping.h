@interface UIImage (Clipping)

/*
 @arg srcRect
 The source rectangle specifying the portion of the image you want to draw. The coordinates of this rectangle are specified in the image's own coordinate system (top left is 0,0).
 */

- (void)drawAtPoint:(CGPoint)point fromRect:(CGRect)srcRect;
- (void)drawAtPoint:(CGPoint)point fromRect:(CGRect)srcRect blendMode:(CGBlendMode)blendMode alpha:(CGFloat)alpha;
- (void)drawInRect:(CGRect)rect fromRect:(CGRect)srcRect;
- (void)drawInRect:(CGRect)rect fromRect:(CGRect)srcRect blendMode:(CGBlendMode)blendMode alpha:(CGFloat)alpha;
- (void)drawAsPatternInRect:(CGRect)rect fromRect:(CGRect)srcRect;

@end
