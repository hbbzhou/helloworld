
//顶点着色器
var VSHADER_SOURCE = 
'attribute vec4 a_Position;\n'+
'attribute vec4 a_Color;\n'+
'varying vec4 v_Color;\n'+
'uniform mat4 u_ModelMatrix;\n'+
'void main(){\n'+
'	gl_Position = u_ModelMatrix * a_Position;\n'+
'	gl_PointSize = 10.0;\n'+
'	v_Color = a_Color;\n'+
'}\n';

//片元着色器
var FSHADER_SOURCE=
'precision mediump float;\n' +
'varying vec4 v_Color;\n' +
'void main(){\n' +
'	gl_FragColor = v_Color;\n' +
'}\n';


var ANGLE_STEP = 45.0;
var MOVE_STEP = 1.0;

function main(){
	//获取<canvas> 元素
	var canvas = document.getElementById('webgl');
	if(!canvas){
		console.log('##Failed to retrieve the <canvas> elemet');
		return;
	}
	
	//获取_xx_绘图上下文
	var gl = getWebGLContext(canvas);
	if(!gl){
		console.log('##Failed to get the rendering context for WebGL ');
		return;
	}
	
	//初始化
	if(!initShaders(gl ,VSHADER_SOURCE ,  FSHADER_SOURCE ) ){
		console.log('##error init');
		return;
	}
	
	var pModelMatrix = gl.getUniformLocation(gl.program , 'u_ModelMatrix');
	var n = initVertexBuffers(gl);
	
	
	if(n < 0){
		console.log('##error n');
		return;
	}
	
	if(pModelMatrix < 0){
		console.log('#error pModelMatrix');
		return;
	}
	
	var curData = [0.0 , 0.0];
	var modelMatrix = new Matrix4();
	var tick = function(){
		curData = animate(curData[0],curData[1]);
		draw(gl,n , curData , modelMatrix , pModelMatrix );
		requestAnimationFrame(tick);
	}
	
	tick();
}

function initVertexBuffers(gl){
	var vertices = new Float32Array([
	0,0.4,0,0,0,1,		0.4,0,0,0,0,1,		0,0,0.4,0,0,1,
	0,0.4,0,0,1.0,0,		0.4,0,0,0,1.0,0,		0,0,0,0,1.0,0,
	0,0.4,0,1,0,1,		0,0,0,1,0,1,		0,0,0.4,1,0,1,
	0,0,0,1,1,0,		0.4,0,0,1,1,0,		0,0,0.4,1,1,0,
	]);
	var n1 = 6;
	var n = vertices.length/n1;
	
	var vertexBuffer = gl.createBuffer();
	if(!vertexBuffer){
		console.log('##error vertexBuffer');
		return -1;
	}
	
	//绑定_缓冲区
	gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
	
	//编辑_缓冲区
	gl.bufferData(gl.ARRAY_BUFFER ,vertices , gl.STATIC_DRAW);
	
	var FSize = vertices.BYTES_PER_ELEMENT;
	
	var pPosition = gl.getAttribLocation(gl.program , 'a_Position');
	if(pPosition < 0){
		console.log('##error , position');
		return -1;
	}
	
	gl.vertexAttribPointer(pPosition , 3 ,gl.FLOAT,false ,FSize*n1,0);
	gl.enableVertexAttribArray(pPosition);
	
	var pColor = gl.getAttribLocation(gl.program, 'a_Color');
	if(pColor < 0){
		console.log('##error pColor');
		return;
	}
	gl.vertexAttribPointer(pColor , 3 , gl.FLOAT,false , FSize*n1 , FSize*3);
	gl.enableVertexAttribArray(pColor);
	
	return n;
}

function draw(gl , n , curData , modelMatrix , pModelMatrix ){
	modelMatrix.setTranslate( Math.cos(curData[1])/2, Math.sin(curData[1])/2, 0.0);
	modelMatrix.rotate(curData[0], 1 , 1 , 0);
	
	gl.uniformMatrix4fv(pModelMatrix , false , modelMatrix.elements);
	
	gl.enable(gl.DEPTH_TEST);
	gl.clearColor(0,0,0,1);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	
	gl.drawArrays(gl.TRIANGLES, 0, n);
}

var g_last = Date.now();
function animate(angle , move1){
	var now = Date.now();
	var elapsed = now - g_last;
	g_last = now;
	var newAngle = angle + (ANGLE_STEP * elapsed) / 1000.0;
	var newMove = move1 + (MOVE_STEP * elapsed) / 1000.0;
	
	return [newAngle %= 360 , newMove%=360];
}










