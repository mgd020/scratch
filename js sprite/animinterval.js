function setAnimationInterval(callback, hz)
{
    var builtin = window.requestAnimationFrame ||
        window.webkitRequestAnimationFrame || 
        window.mozRequestAnimationFrame ||
        window.oRequestAnimationFrame ||
        window.msRequestAnimationFrame;
    
    if (!builtin)
    return window.setInterval(function() { callback(new Date) }, 1000/(hz ? hz : 60));
    
    var f = function(time) { callback(time); builtin(f); }
    builtin(callback ? f : null);
}

function clearAnimationInterval(interval)
{
    var builtin = window.clearRequestAnimationFrame ||
        window.webkitclearRequestAnimationFrame || 
        window.mozClearRequestAnimationFrame ||
        window.oClearRequestAnimationFrame ||
        window.msClearRequestAnimationFrame;
    
    if (builtin)
        builtin(interval);
    else
        window.clearInterval(interval);
}
