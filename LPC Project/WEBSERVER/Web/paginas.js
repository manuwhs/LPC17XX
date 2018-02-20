function pagina_motores(){

	document.body.innerHTML = "<br> <h1 >Control de los motores</h1>"+
	"<div id='screen' name = 'pantalla'>  </div>";	
	document.body.innerHTML += "<form method='post' name = 'super_form' action = 'webs.cgi'>";	
	
	CreateInp_send(40,20,"menu_change","0");
	CreateDiv(40+40,20, "Motores");

	CreateInp_send(200,20,"menu_change","1");	
	CreateDiv(200+40,20, "Camara");
		
	num_bars = 0;
	num_texts = 0;	
	
	var ox = screenX + screenW;
	var oy = screenY + screenH;

	CreateDiv(screenX+det_X,screenY+det_Y,"");
	CreateDiv(screenX+det_X,screenY+det_Y,"X");
	
	CreateBar(screenX - 30, screenY + 20, 20,screenH -40,1);
	CreateBar(screenX + 20, screenY + screenH + 10, screenW-40,20,0);
	
	CreateDiv(ox+10,screenY + 10, ""+   
	"<h3> Configuración de Detección:</h3>"+
	"<div style='margin-left:30px;text:20px'>"+
	"<input type='radio' name='seg' value='0'>Seguimiento <br>"+
	"<input type='radio' name='seg' value='1'>Movimiento Manual</div>"
	);
	
	inpurrus = super_form.getElementsByTagName("input");
	for (i = 0; i < inpurrus.length; i++){
		if (inpurrus[i].type == "radio"){	
			inpurrus[i].onclick= function(event){ 
				document.super_form.submit();
			}
		}	
		if (inpurrus[i].name == "seg"){
			if (inpurrus[i].value == seguimiento.toString()){
				inpurrus[i].checked = 1;
			}	
		}
	}
	
	barritas = document.getElementsByTagName("div");
		for (i = 0; i < barritas.length; i++){
			nombre_barra_aux = barritas[i].id;
			nombre_barra_aux = nombre_barra_aux.substring(0,5);

		if (nombre_barra_aux == "barra"){	
		barritas[i].onclick= function(event){ 
		coordenadas(event);
		var auxil = getBarValue(this,type_bars[parseInt(this.name)]);
		DrawBar(this,auxil,type_bars[parseInt(this.name)]);
		aux_num_bar = parseInt(this.name);
		input_barra_n = "barra_inp" + parseInt((this.name)).toString();
		document.getElementById(input_barra_n).value = value_bars[aux_num_bar];	
		document.super_form.submit(); 
		   }
		}
	}

	CreateDiv(screenX,screenY - 30, ""+ 		
	" Posición de los motores: "+
	" X: <input type='text' id='x' name = 'x' class = 'coord'  readonly>"+
	" Y: <input type='text' id='y' name = 'y' class = 'coord'  readonly>"
	);
	
	if(seguimiento == 0){
	setInterval("document.super_form.submit()",1000);
	}
	Draw_detected(det_x,det_y);
	document.body.innerHTML += "</form>";	

	document.super_form.getElementById("x").value = det_X;	
	document.super_form.getElementById("y").value = det_Y;	
	
}

function pagina_camara(){

	document.body.innerHTML = "<br> <h1 >Control de la camara</h1>"+
	"<div id='screen' name = 'pantalla'>  </div>";

	document.body.innerHTML += "<form method='post' name = 'super_form' action = 'webs.cgi'>";	
	CreateInp_send(40,20,"menu_change","0");
	CreateDiv(40+40,20, "Motores");

	CreateInp_send(200,20,"menu_change","1");	
	CreateDiv(200+40,20, "Camara");	

	num_bars = 0;
	num_texts = 0;	
	
	var ox = screenX + screenW;
	var oy = screenY + screenH;
	
	for (i = 0; i < 3; i++){
		CreateBar( ox + 20 + 40*i, screenY + 20,15,150,1);
		CreateDiv(ox + 20 + 40*i,screenY, textos[i])
	}
	
	for (i = 0; i < 3; i++){
		CreateBar( ox + 20 + 40*i + 200, screenY + 20, 15,150,1);
		CreateDiv(ox + 20 + 40*i + 200,screenY, textos[3+i])
	}
	
	CreateBar( screenX + 20 , oy + 20, 15,100,1);	
	
	CreateDiv(screenX,screenY - 30,
	" Número de FPS recibidos: "+
	" <input type='text' id='real_fps' name = 'real_fps' class = 'coord' readonly>"
	);
	
	CreateDiv(ox+10,oy - 60, ""+
	"Ganancia RGB"+
	"<div style='margin-left:30px;text:20px'>"+
	"<input type='radio' name='RGB' value='0'>Manual <br>"+
	"<input type='radio' name='RGB' value='1'>Automatico</div>"
	);
	
	CreateDiv(ox+210,oy - 60, ""+ 
	"Brillo, Contraste y Luminosidad"+
	"<div style='margin-left:30px;text:20px'>"+
	"<input type='radio' name='BCL' value='0'>Manual <br>"+
	"<input type='radio' name='BCL' value='1'>Automatico</div>"
	);
	
	CreateDiv(screenX+50,oy +20,"Umbral de deteccion"+
	"<div id='color_detected' style = 'position:absolute;left:25px; top:35px;"+
	"width:40px;height:40px; border:1px black solid;background:red'>  </div>"
	
	);		
	CreateDiv(screenX - 130,screenY,""+	
	"Número de FPS"+
	"<div style='margin-left:50px;text:20px'>"+
	"<input type='radio' name='fps' value='1'>1 <br>"+
	"<input type='radio' name='fps' value='2'>2 <br>"+
	"<input type='radio' name='fps' value='3'>3 <br>"+
	"<input type='radio' name='fps' value='5'>5 <br>"+
	"<input type='radio' name='fps' value='10'>10 <br>"+
	"<input type='radio' name='fps' value='15'>15 <br>"+
	"<input type='radio' name='fps' value='20'>20 <br>"+
	"<input type='radio' name='fps' value='30'>30 <br>"+		
	"<input type='radio' name='fps' value='40'>40 <br>"
	);
	inpurrus = super_form.getElementsByTagName("input");
	
	for (i = 0; i < inpurrus.length; i++){
		if (inpurrus[i].type == "radio"){	
			inpurrus[i].onclick= function(event){ 
				document.super_form.submit();
			}
		}	
		if (inpurrus[i].name == "fps"){
			if (inpurrus[i].value == fps_num.toString()){
				inpurrus[i].checked = 1;
			}	
		}
		if (inpurrus[i].name == "RGB"){
			if (inpurrus[i].value == RGB_modo.toString()){
				inpurrus[i].checked = 1;
			}	
		}		
		if (inpurrus[i].name == "BCL"){
			if (inpurrus[i].value == BCL_modo.toString()){
				inpurrus[i].checked = 1;
			}	
		}			
	}	
	barritas = document.getElementsByTagName("div");
		for (i = 0; i < barritas.length; i++){
			nombre_barra_aux = barritas[i].id;
			nombre_barra_aux = nombre_barra_aux.substring(0,5);

		if (nombre_barra_aux == "barra"){	
		barritas[i].onclick= function(event){ 
		coordenadas(event);
		var auxil = getBarValue(this,type_bars[parseInt(this.name)]);
		DrawBar(this,auxil,type_bars[parseInt(this.name)]);
		aux_num_bar = parseInt(this.name);
		input_barra_n = "barra_inp" + parseInt((this.name)).toString();
		document.getElementById(input_barra_n).value = value_bars[aux_num_bar];	
		document.super_form.submit(); 
		   }
		}
	}
	
	//	document.body.innerHTML += "</form>"; // NO ME DEJA PONERLO ???
}