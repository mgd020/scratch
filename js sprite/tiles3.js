function TileSet(image, tileWidth, tileHeight, tileSpacing)
{
    this.image = image;
    this.tileWidth = tileWidth;
    this.tileHeight = tileHeight ? tileHeight : tileWidth;
    this.tileSpacing = tileSpacing ? tileSpacing : 0;
}

// drawTile(context, tileX, tileY, drawX, drawY)
// drawTile(context, tileX, tileY, drawX, drawY, drawW, drawH)
// drawTile(context, tileX, tileY, clipX, clipY, clipW, clipH, drawX, drawY)
// drawTile(context, tileX, tileY, clipX, clipY, clipW, clipH, drawX, drawY, drawW, drawH)
TileSet.prototype.drawTile = function()
{
    var dx, dy, dw, dh, sx, sy, sw, sh;
    
    if (arguments.length == 5)
    {
        sx = 0;
        sy = 0;
        sw = this.tileWidth;
        sh = this.tileHeight;
        dx = arguments[3];
        dy = arguments[4];
        dw = this.tileWidth;
        dh = this.tileHeight;
    }
    else if (arguments.length == 7)
    {
        sx = 0;
        sy = 0;
        sw = this.tileWidth;
        sh = this.tileHeight;
        dx = arguments[3];
        dy = arguments[4];
        dw = arguments[5];
        dh = arguments[6];
    }
    else if (arguments.length == 9)
    {
        sx = arguments[3];
        sy = arguments[4];
        sw = arguments[5];
        sh = arguments[6];
        dx = arguments[7];
        dy = arguments[8];
        dw = this.tileWidth;
        dh = this.tileHeight;
    }
    else if (arguments.length == 11)
    {
        sx = arguments[3];
        sy = arguments[4];
        sw = arguments[5];
        sh = arguments[6];
        dx = arguments[7];
        dy = arguments[8];
        dw = arguments[9];
        dh = arguments[10];
    }
    
    sx += arguments[1] * (this.tileWidth + this.tileSpacing);
    sy += arguments[2] * (this.tileHeight + this.tileSpacing);
    
    arguments[0].drawImage(this.image, sx, sy, sw, sh, dx, dy, dw, dh);
}

function TileMap(tileSet, width, height)
{
    this.tileSet = tileSet;
    this.width = width;
    this.height = height;
    this.tiles = new Array();
}

TileMap.prototype.setTile = function(tile, x, y, z)
{
    if (!z)
        z = 0;
    
    if (!this.tiles[z])
        this.tiles[z] = new Array();
    
    var layer = this.tiles[z];
    
    if (!layer[y])
        layer[y] = new Array();
    
    var line = layer[y];
    
    if (!line[x])
        line[x] = new Array();
    
    line[x] = tile;
}

// draw(context, drawX, drawY, drawW, drawH)
// draw(context, clipX, clipY, clipW, clipH, drawX, drawY, drawW, drawH)
TileMap.prototype.draw = function()
{
    var dx, dy, dw, dh, cx, cy, cw, ch;
    
    if (arguments.length == 5)
    {
        cx = 0;
        cy = 0;
        cw = this.width * this.tileSet.tileWidth;
        ch = this.height * this.tileSet.tileHeight;
        dx = arguments[1];
        dy = arguments[2];
        dw = arguments[3];
        dh = arguments[4];
    }
    else if (arguments.length == 9)
    {
        cx = arguments[1];
        cy = arguments[2];
        cw = arguments[3];
        ch = arguments[4];
        dx = arguments[5];
        dy = arguments[6];
        dw = arguments[7];
        dh = arguments[8];
    }
    
    // the size of tiles
    var tx = ;
    var ty = ;
    var tw = dw / cw;
    var th = dh / ch;
    
    // draw layers (back to front)
    for (var z in this.tiles)
    {
        var layer = this.tiles[z];
                 
        // draw lines (top to bottom)
        for (var y=Math.floor(this.viewport.y),dy=Math.ceil(this.viewport.y + this.viewport.height); yy<dy; ++yy)
        {
            var line = layer[yy];
            
            if (!line)
                continue;
            
            // draw tiles (left to right)
            for (var xx=Math.floor(this.viewport.x),dx=Math.ceil(this.viewport.x + this.viewport.width); xx<dx; ++xx)
            {
                var tile = line[xx];
                
                if (!tile)
                    continue;
                
                var tx = xx * tw + x;
                var ty = yy * th + y;
                
                this.tileSet.drawTile(arguments[0], tile.x, tile.y, tx, ty, tw, th);
            }
        }
    }
}
