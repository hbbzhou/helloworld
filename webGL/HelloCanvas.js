
function main(){
	//获取<canvas> 元素
	var canvas = document.getElementById('webgl');
	if(!canvas){
		console.log('Failed to retrieve the <canvas> elemet');
		return;
	}
	
	//获取绘制二维图形的绘图上下文
	var gl = getWebGLContext(canvas);
	if(!gl){
		console.log('Failed to get the rendering context for WebGL ');
		return;
	}
	gl.clearColor(0.0,0.0,0.0,1.0);
	
	gl.clear(gl.COLOR_BUFFER_BIT);
}


