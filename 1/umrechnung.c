#include <stdio.h>

main()
{
    double k = 9/5;                             /* Gleitkommakonstante zum Umrechnen */

    printf("Temperatur in Grad Celsius: ");
    double celsius;                             /* Gleitkommavariable */
    scanf("%lf", &celsius);

    double fahrenheit = k * celsius + 32;

    printf("...ergibt in Fahrenheit: %6.2lf\n", fahrenheit);
}
