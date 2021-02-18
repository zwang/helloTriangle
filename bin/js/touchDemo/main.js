var width = 0;
var height = 0;
var program_texture = 0;
var vertexBuffer = 0;
var uResolution = 0;
var uColor = 0;
var aPosition = 0;

let BoxSize = 50;

var boxes = [];

function distance(x1, y1, x2, y2) {
    return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
}

function addBox(x, y) {
    boxes.push({
        x: x - BoxSize,
        y: y - BoxSize,
        width: BoxSize,
        height: BoxSize,
        color: [
            Math.random(),
            Math.random(),
            Math.random(),
            1,
        ],
    });
    plato.log("after addBox: " + boxes);
    drawFrame();
}

function clearBoxes() {
    boxes.length = 0;
    drawFrame();
}

function onClick(posX, posY) {
    plato.log("onClick(x: " + posX + ", " + posY + ")");
    clearBoxes()
    addBox(posX, posY);
}

function initSurface() {
    plato.log("SURFACE CREATE", 1);
    init(1, 1, 0, 1);
    resize(640, 480)
}

function loadShader(type, shader_code) {
    var shader = GL.createShader(type);
    GL.shaderSource(shader, shader_code);
    GL.compileShader(shader);
    var compileStatus = new Int32Array(1)
    GL.getShaderiv(shader, GL.COMPILE_STATUS, compileStatus);
    if (compileStatus[0] === GL.FALSE) {
        plato.log("Compile Shader failed");
    }
    plato.log("Compile Shader STATUS: " + compileStatus[0]);
    return shader;
};

function init(r, g, b, a) {
    GL.clearColor(r, g, b, a);

    var vertex_shader_code = "precision mediump float;\n" +
        "\n" +
        "uniform vec2 u_resolution;\n" +
        "\n" +
        "attribute vec2 a_position;\n" +
        "\n" +
        "void main() {\n" +
        "\t// convert the position from pixels to 0.0 to 1.0\n" +
        "\tvec2 zeroToOne = a_position / u_resolution;\n" +
        "\n" +
        "\t// convert from 0->1 to 0->2\n" +
        "\tvec2 zeroToTwo = zeroToOne * 2.0;\n" +
        "\n" +
        "\t// convert from 0->2 to -1->+1 (clipspace)\n" +
        "\tvec2 clipSpace = zeroToTwo - 1.0;\n" +
        "\n" +
        "\tgl_Position = vec4(clipSpace * vec2(1, -1), 0, 1);\n" +
        "}"
    var fragment_shader_code = "precision mediump float;\n" +
        "\n" +
        "uniform vec4 u_color;\n" +
        "\n" +
        "void main() {\n" +
        "\tgl_FragColor = u_color;\n" +
        "}\n"
    program_texture = GL.createProgram();
    GL.attachShader(program_texture, loadShader(GL.VERTEX_SHADER, vertex_shader_code));
    GL.attachShader(program_texture, loadShader(GL.FRAGMENT_SHADER, fragment_shader_code));
    GL.linkProgram(program_texture);
    var linkStatus = new Int32Array(1);
    GL.getProgramiv(this.program_texture, GL.LINK_STATUS, linkStatus );
    if (linkStatus[0] === GL.FALSE) {
        plato.log("LinkProgram failed");
    }
    plato.log("link program: " + program_texture + ", STATUS: " + linkStatus[0]);


    uResolution = GL.getUniformLocation(this.program_texture, "u_resolution");
    uColor = GL.getUniformLocation(this.program_texture, "u_color");
    aPosition = GL.getAttribLocation(this.program_texture, "a_position");

    plato.log("uResolution: " + uResolution + ", uColor: " + uColor + ", aPosition: " + aPosition)

    // Create buffers
    var buffers = new Int32Array(1);
    GL.genBuffers(buffers.length, buffers);
    vertexBuffer = buffers[0];
    plato.log("vertexBuffer: " + vertexBuffer + ", buffers: " + buffers)
}

function resize(width, height) {
    this.width = width;
    this.height = height;
    GL.viewport(0, 0, width, height);
    plato.log("resize width: " + width + ", height: " + height);
}

function drawFrame() {
    beginFrame()
    var th = 5;
    var hh = Math.floor(th / 2);
    // plato.log("drawFrame, hh: " + hh);
    drawRect(0, 20 - hh, this.width, th, [1, 0, 1, 1]);
    drawRect(0, this.height - hh, this.width, th, [0, 1, 0, 1]);
    var kbY = this.height - hh;
    // plato.log("drawFrame, kbY: " + kbY);
    drawRect(0, kbY, this.width, th, [0, 1, 1, 1]);
    var rh = Math.floor(this.height * 0.25);
    //plato.log("drawFrame, rh: " + rh);
    drawRect(0, this.height / 2 - rh / 2, this.width, rh, [0, 0, 1, 1]);

    var i;
    for (i = 0; i < boxes.length; i++) {
        var box = boxes[i];
        drawRect(box.x, box.y, box.width, box.height, box.color);
    }
}

function beginFrame() {
    GL.clear(GL.COLOR_BUFFER_BIT);
    GL.useProgram(this.program_texture);
    GL.uniform2f(this.uResolution, this.width, this.height);
}

function drawRect(x, y, w, h, colorArray) {
    const geom = new Float32Array([
        x, y,
        x + w, y,
        x + w, y + h,
        x + w, y + h,
        x, y + h,
        x, y,
    ]);

    // plato.log("vertexBuffer: " + vertexBuffer)
    GL.bindBuffer(GL.ARRAY_BUFFER, this.vertexBuffer);
    GL.bufferData(GL.ARRAY_BUFFER, geom.byteLength, geom, GL.STATIC_DRAW);

    GL.uniform4f(this.uColor, colorArray[0], colorArray[1], colorArray[2], colorArray[3]);

    GL.enableVertexAttribArray(this.aPosition);
    GL.vertexAttribPointer(this.aPosition, 2, GL.FLOAT, false, 0, 0);

    GL.drawArrays(GL.TRIANGLES, 0, Math.floor(geom.length / 2));
}
