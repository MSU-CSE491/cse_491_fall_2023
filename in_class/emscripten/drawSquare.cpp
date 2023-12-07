#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <iostream>

int X_pos=50, Y_pos=50;

extern "C" {
  // Function to be called from JavaScript
  EMSCRIPTEN_KEEPALIVE
  void drawSquare() {
    EM_ASM_({
      var canvas = document.getElementById('canvas');
      if (canvas.getContext) {
        var ctx = canvas.getContext('2d');
        ctx.fillStyle = 'red';
        ctx.fillRect($0, $1, 100, 100); // Draw a 100x100 red square
      }
    }, X_pos, Y_pos);
    X_pos += 10;
    Y_pos += 10;
  }
}

int main() {
  // Use EM_ASM to inject HTML for the button
  EM_ASM({
    var button = document.createElement('button');
    button.innerHTML = 'Draw Square';
    button.onclick = function() {
      // Call the C++ function drawSquare()
      _drawSquare();
    };
    document.body.appendChild(button);
      
    // Create a canvas element
    var canvas = document.createElement('canvas');
    canvas.id = 'canvas';
    canvas.width = 300;
    canvas.height = 300;
    canvas.style.border = '1px solid black';
    document.body.appendChild(canvas);
  });
  
  return 0;
}
