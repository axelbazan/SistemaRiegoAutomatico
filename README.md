# Sistema de Riego Automatico

Diseño de un sistema de riego automatizado para el vivero de plantas nativas de la UNICEN.

Las plantas nativas se cultivan a la intemperie en pequeños plantines que son ubicados en bancales. Al momento de regar, el bancal es inundado hasta la mitad de la altura de los plantines y se deja el agua estancada durante 20 minutos, con el objetivo de que el agua suba desde el fondo de las macetas. Esta alternativa es mejor que el riego desde arriba con regadera o manguera, ya que de esa manera se humedece el follaje y hace que las plantas sean más propensas a la propagación de hongos. Pasado el tiempo de remojo, el bancal es vaciado en forma automatizada.

El sistema usa una placa arduino, conectada a un sensor de ultrasonido para medir el nivel de agua en el bancal, se conecta también a las electroválvulas de llenado y las bombas de vaciado. Para optimizar el horario de riego en verano y en invierno, se agregó un módulo de reloj que permite registrar la fecha y la hora a lo largo del tiempo.

