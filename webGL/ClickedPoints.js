
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
	
	var pPosition = gl.getAttribLocation(gl.program , 'a_Position');
	var pSize = gl.getAttribLocation(gl.program , 'a_Size');
	var pFragColor = gl.getUniformLocation(gl.program , 'u_FragColor');
	if(pPosition < 0){
		console.log('##error , position');
		return;
	}
	
	if(pSize < 0){
		console.log('##error , pSize');
		return;
	}
	
	if(pFragColor < 0){
		console.log('##error pFragColor');
		return;
	}
	
	//注册鼠标事件_回调
	canvas.onmousedown = function(ev){click(ev ,gl , canvas , pPosition , pFragColor);};
	
	gl.vertexAttrib4f(pPosition , 0.0,0.0,0.0 ,1.0);
	gl.vertexAttrib1f(pSize , 5.0);
	
	//开始_绘制
	
	//绘制_背景
	gl.clearColor(0.0,0.0,0.0,1.0);
	gl.clear(gl.COLOR_BUFFER_BIT);
	
}


var g_points= [];
var g_colors= [];
function click(ev , gl , canvas , pPosition , pFragColor){
	var x = ev.clientX;
	var y = ev.clientY;
	var rect = ev.target.getBoundingClientRect();
	x = ((x-rect.left) - canvas.width/2)/(canvas.width/2);
	y = (canvas.height/2 - (y-rect.top))/(canvas.height/2);
	g_points.push([x,y])
	
	if(x >=0.0 && y >= 0.0){
		g_colors.push([1.0,0.0,0.0,1.0]);
	}
	else if(x< 0.0 && y < 0.0){
		g_colors.push([0.0,1.0,0.0,1.0]);
	}
	else{
		g_colors.push([1.0,1.0,1.0,1.0]);
	}
	
	
	gl.clear(gl.COLOR_BUFFER_BIT);
	var len = g_points.length;
	for(var i =0 ; i<len ; i++){
		var x_y = g_points[i];
		var agba = g_colors[i];
		gl.uniform4f(pFragColor , agba[0],agba[1],agba[2],agba[3]);
		gl.vertexAttrib4f(pPosition , x_y[0] , x_y[1] , 0.0 , 1.0);
		gl.drawArrays(gl.POINTS, 0 , 1);
	}
}








