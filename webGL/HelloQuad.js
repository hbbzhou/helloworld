
//顶点着色器
var VSHADER_SOURCE = 
'attribute vec4 a_Position;\n'+
'void main(){\n'+
'	gl_Position = a_Position;\n'+
'}\n';

//片元着色器
var FSHADER_SOURCE=
'precision mediump float;\n' +
'uniform vec4 u_FragColor;\n' +
'void main(){\n' +
'	gl_FragColor = u_FragColor;\n' +
'}\n';



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
	
	var pFragColor = gl.getUniformLocation(gl.program , 'u_FragColor');
	var n = initVertexBuffers(gl);
	
	if(pFragColor < 0){
		console.log('##error pFragColor');
		return;
	}
	
	if(n < 0){
		console.log('##error n');
		return;
	}
	
	gl.uniform4f(pFragColor , 1.0,0.0,0.0,1.0);
	//开始_绘制
	
	//绘制_背景
	gl.clearColor(0.0,0.0,0.0,1.0);
	gl.clear(gl.COLOR_BUFFER_BIT);
	
	gl.drawArrays(gl.TRIANGLE_STRIP , 0, n);
}

function initVertexBuffers(gl){
	var vertices = new Float32Array([-0.5,0.5, -0.5,-0.5, 0.5,0.5, 0.5,-0.5]);
	var n = 4;
	
	var vertexBuffer = gl.createBuffer();
	if(!vertexBuffer){
		console.log('##error vertexBuffer');
		return -1;
	}
	
	//绑定_缓冲区
	gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
	
	//编辑_缓冲区
	gl.bufferData(gl.ARRAY_BUFFER ,vertices , gl.STATIC_DRAW);
	
	var pPosition = gl.getAttribLocation(gl.program , 'a_Position');
	if(pPosition < 0){
		console.log('##error , position');
		return -1;
	}
	
	gl.vertexAttribPointer(pPosition , 2,gl.FLOAT,false ,0,0);
	gl.enableVertexAttribArray(pPosition);
	return n;
}








