function TileSet(image, tw, th)
{
    this.image = image;
    this.sw = tw;
    this.sh = th;
    this.tw = Math.floor((image.width + 1) / (tw + 1));
}

TileSet.prototype.drawTile = function(tile, context, dx, dy, dw, dh)
{
    var sx = (tile % this.tw) * (this.sw + 1);
    var sy = Math.floor(tile / this.tw) * (this.sh + 1);
    dw = dw ? dw : this.sw;
    dh = dh ? dh : this.sh;
    context.drawImage(this.image, sx, sy, this.sw, this.sh, dx, dy, dw, dh);
}

function TileMap(tileSet, w, h)
{
    this.tileSet = tileSet;
    this.width = w;
    this.height = h;
    this.viewport = {x:0, y:0, width:w, height:h};
    this.tiles = new Array(new Array(new Array()));
}

TileMap.prototype.setViewport = function(x, y, width, height)
{
    this.viewport.x = x;
    this.viewport.y = y;
    if (width)
        this.viewport.width = width;
    if (height)
        this.viewport.height = height;
}

TileMap.prototype.tile = function(x, y, layer)
{
    return this.tiles[layer ? layer : 0][y][x];
}

TileMap.prototype.draw = function(context, dx, dy, dw, dh)
{
    // the size of tiles
    var tw = dw / this.viewport.width;
    var th = dh / this.viewport.height;
    
    // draw only visible tiles
    var x0 = Math.floor(this.viewport.x);
    var dx = Math.ceil(this.viewport.x + this.viewport.width);
    var y0 = Math.floor(this.viewport.y);
    var dy = Math.ceil(this.viewport.y + this.viewport.height);
    
    // draw layers (back to front)
    for (var layer in this.tiles)
    {
        // draw lines (top to bottom)
        for (var y=y0; y<dy; ++y)
        {
            if (!layer[y])
                continue;
            
            // draw tiles (left to right)
            for (var x=x0; x<dx; ++x)
            {
                if (!layer[y][x])
                    continue;
                
                this.tileSet.drawTile(layer[y][x], context, x, y, tw, th);
            }
        }
    }
}
