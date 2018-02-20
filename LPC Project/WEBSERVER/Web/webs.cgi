t <html><head>
t <script type="text/javascript" src="funciones.js"> </script>
t <script type="text/javascript" src="paginas.js"> </script>
t <link rel="stylesheet" type="text/css" href="estilo.css">
t <script type="text/javascript">
c e value_bars[0] = %d;
c d value_bars[1] = %d;
c j value_bars[2] = %d;
c i value_bars[3] = %d;
c h value_bars[4] = %d;
c k value_bars[5] = %d;
c l value_bars[6] = %d;
c f var menu = %d;		
c b var det_X = %d;
c c var det_Y = %d;
c a var seguimiento = %d;
c m var BCL_modo = %d;
c n var RGB_modo = %d;
c g var fps_num = %d;
t 
# value_bars[0] = 20;
# value_bars[1] = 10;
# value_bars[2] = 25;
# value_bars[3] = 75;
# value_bars[4] = 45;
# value_bars[5] = 75;
# value_bars[6] = 50;
# var menu = 0;		
# var det_X = 10;
# var det_Y = 22;
# var seguimiento = 0;
# var BCL_modo = 0;
# var RGB_modo = 1;
# var fps_num = 15;
t </script>
t </head>
t <body></body>
t <script type="text/javascript">
t if (menu == 1)
t 	pagina_camara();
t if (menu == 0)
t 	pagina_motores();
t </script>
t </html>
. End of script must be closed with period.
