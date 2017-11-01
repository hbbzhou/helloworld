
//顶点着色器
var VSHADER_SOURCE = 
'attribute vec4 a_Position;\n'+
'attribute float a_Size;\n'+
'void main(){\n'+
'	gl_Position = a_Position;\n'+
'	gl_PointSize = a_Size;\n'+
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
	
	var pPosition = gl.getAttribLocation(gl.program , 'a_Position');
	var pSize = gl.getAttribLocation(gl.program , 'a_Size');
	if(pPosition < 0){
		console.log('##error , position');
		return
	}
	
	if(pSize < 0){
		console.log('##error , pSize');
		return
	}
	
	gl.vertexAttrib4f(pPosition , 0.0,0.0,0.0 ,1.0);
	gl.vertexAttrib1f(pSize , 50.0);
	
	gl.clearColor(0.0,0.0,0.0,1.0);
	gl.clear(gl.COLOR_BUFFER_BIT);
	
	//绘制一个点
	gl.drawArrays(gl.POINTS,0,1);
	
}


