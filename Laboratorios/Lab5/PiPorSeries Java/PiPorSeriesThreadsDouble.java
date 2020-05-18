/**
 * La clase PiPorSerieThreads lleva a cabo calculos de aproximación de PI usando threads
 *
 * @author Programacion Concurrente
 * @version 2020/Abr/11
 *       __
 *      \
 * pi =  >  (-1)^i x 4/(2xi + 1)
 *      /__
 *      i=0
 *
 */

/**
 * Esta es la clase que vamos a correr cada vez que se crea un hilo
 * Se utiliza el metodo "run" para realizar la tarea de cada hilo
 *
 */
class SeriePorPartes implements Runnable {
    private long fin;
    private long inicio;
    private Double miDouble[];
    private int i;

/**
 * En el constructor se definen los limites para los que este hilo realiza
 * el calculo de terminos
 * @param	long	inicio, es el punto donde inicia la suma que calcula este hilo
 * @param	long	fin, es la cantidad de terminos que le toca calcular a este hilo
 * @param	Double	miParte, una referencia de Suma para guardar el resultado de este hilo
 */
    public SeriePorPartes( long inicio, long fin, Double miDouble[], int i ) {
        this.inicio = inicio;
        this.fin = fin;
        this.miDouble = miDouble;
        this.i = i;
    }

/**
 * Este metodo es el que ejecuta cada hilo que se crea
 * Realiza la suma de los terminos que van desde el valor "inicial"
 * hasta la cantidad de "terminos"
 * Coloca el resultado en una instancia de la clase Suma
 */
    public void run() {
       double alterna;
       double res = 0.0;

       alterna = ( 0 == ( inicio % 2 ) )? 4 : -4;
       for ( long i = inicio; i < fin; i++ ) {
           res += alterna/(2 * i + 1);
           alterna *= -1;
       }

      miDouble[i] = new Double(res);
    }
}

public class PiPorSeriesThreadsDouble {


/**
 * Realiza las pruebas de la clase
 */
    public static void main( String[] args ) {
        long n = 100000;
        double x = 0;
        Thread thread[] = new Thread[ 10 ];	// Una instancia por cada hilo creado
                  // Se utilizan para esperar por las sumas de cada hilo
        Double miDouble[] = new Double[10];

	if ( args.length > 0 ) {
           n = Long.parseLong( args[ 0 ] );
	}
        for ( int i = 0; i < 10; i++ ) {	// Generar los 10 hilos calculadores
           long inicio = i * n / 10;
           long fin = (i+1)*n/10;
           thread[ i ] = new Thread( new SeriePorPartes( inicio, fin, miDouble, i) );
           thread[ i ].start();
        }

        for ( int i = 0; i < 10; i++ ) {	// Esperar por los resultados de cada hilo
           try {
              thread[ i ].join();
              x += miDouble[i].doubleValue();
//    System.out.println("Valor aportado por hilo " + i + " es " + miParte[ i ].getSuma() );
           } catch ( InterruptedException e) {
           }
        }
        System.out.println("Calculo de PiPorSeries utilizando un vector de objetos Double\n");
        System.out.println( "Valor calculado de PI: " + x );

    }
}


