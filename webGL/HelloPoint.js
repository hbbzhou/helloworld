
//顶点着色器
var VSHADER_SOURCE = 
'void main(){\n'+
'	gl_Position = vec4(0.0,0.0,0.0,1.0);\n'+
'	gl_PointSize = 10.0;\n'+
'}\n';

//片元着色器
var FSHADER_SOURCE=
'void main(){\n' +
'	gl_FragColor = vec4(1.0,0.0,0.0,1.0);\n' +
'}\n';


function main(){
	//获取<canvas> 元素
	var canvas = document.getElementById('webgl');
	if(!canvas){
		console.log('Failed to retrieve the <canvas> elemet');
		return;
	}
	
	//获取_xx_绘图上下文
	var gl = getWebGLContext(canvas);
	if(!gl){
		console.log('Failed to get the rendering context for WebGL ');
		return;
	}
	
	//初始化
	if(!initShaders(gl ,VSHADER_SOURCE ,  FSHADER_SOURCE ) ){
		console.log('error 1');
		return;
	}
	
	gl.clearColor(0.0,0.0,0.0,1.0);
	gl.clear(gl.COLOR_BUFFER_BIT);
	
	//绘制一个点
	gl.drawArrays(gl.POINTS,0,1);
	
}


