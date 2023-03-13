#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
// inicializa todas las structs 
struct FECHA{
  int dia;
  int mes;
  int anio;
};
struct HORA {
  int hora;
  int minuto;
  int segundo;
};
struct DATO{
  float temp;
  struct FECHA fecha;
  struct HORA hora;
  int tendencia;
};

struct lista_temp{
  struct DATO dato;
  struct lista_temp *siguiente;
};
// crea un nodo para comparar la temperatura utilizando listas
struct lista_temp *NUEVO_NODO() {
  return (struct lista_temp *)malloc(sizeof(struct lista_temp));
}
struct lista_temp *NUEVA_TEMP(struct lista_temp *lista, struct DATO datos) {
  //Agrega una temperatura al inicio de la lista
  struct lista_temp *q, *aux;
  q = NUEVO_NODO();
  aux=NUEVO_NODO();
  q->dato=datos;
  q->siguiente=NULL;
  aux=lista;
  if(lista==NULL) {
    lista=q;
  } else {
    while(aux->siguiente != NULL) {
      aux=aux->siguiente;
    }
  aux->siguiente=q;
  }
  return lista;
}

void IMPRIMIR_LISTA(struct lista_temp *lista) {
  struct lista_temp *p;
  int elemento=0;
  char tendencia_str[20];
  p=lista;
  while(p != NULL){
    elemento++;
    switch(p->dato.tendencia) {
      case 0:
        strcpy(tendencia_str,"Baja");
        break;
      case 1:
        strcpy(tendencia_str,"Estable");
        break;
      case 2:
        strcpy(tendencia_str,"Alta");
        break;
    }
    printf("Temperatura %d: %.1f°C - Tendencia = %s - Hora: %02d:%02d:%02d - %02d/%02d/%d \n",elemento, p->dato.temp, tendencia_str, p->dato.hora.hora, p->dato.hora.minuto, p->dato.hora.segundo, p->dato.fecha.dia, p->dato.fecha.mes, p->dato.fecha.anio );
    p=p->siguiente;
  }
}

struct lista_temp *ELIMINAR_TEMP(struct lista_temp *lista, float temperatura) {
  struct lista_temp *aux, *ant;
  aux=lista;
  ant=NULL;

  while(aux != NULL){
    if(aux->dato.temp == temperatura) {
      if(ant){
        ant->siguiente=aux->siguiente;
      } else {

        lista = aux->siguiente;
      }
      free(aux);
      break;
    } else {
      ant=aux;
      aux=aux->siguiente;
    }
  }

  return(lista);
}

float CALC_MEDIA(struct lista_temp *lista) {
  struct lista_temp *aux;
  float media=0;
  int contador=0;
  aux=lista;
  while(true) {
    contador++;
    media += aux->dato.temp;

    if (aux->siguiente != NULL) {
      aux=aux->siguiente;
    } else {
      break;
    }
  }
  media = media / contador;
  printf("La temperatura media es %.1f°C\n", media);
  return media;
}

float CALC_MODA(struct lista_temp *lista) {
  struct lista_temp *aux1, *aux2;
  aux1=lista;
  aux2=lista;
  float moda=0;
  int frec_max=0;
  int frec=0;
  while(true) {
    while(true) {
      if(aux1->dato.temp==aux2->dato.temp) {

        frec++;
      }
      if(frec>frec_max) {
        moda=aux1->dato.temp;
        frec_max=frec;
      }
      if(aux2->siguiente != NULL) {
        aux2=aux2->siguiente;
      } else {
        break;
      }
    }
    if(aux1->siguiente != NULL) {
      aux1=aux1->siguiente;
      aux2=lista;
      frec=0;
    } else {
      break;
    }
  }
  printf("La temperatura modal es %.1f°C. ", moda);
  printf("Se repitió %d veces.\n", frec_max);
  return moda;
}

struct lista_temp *ordenar_lista(struct lista_temp *lista) {
  struct lista_temp *aux, *lista_ord, *aux2;
  aux=lista;
  aux2=lista;
  lista_ord=NULL;
  float temp_minima=200;
  struct DATO dato_ord;

  while(true) {
    temp_minima=aux->dato.temp;
    while(true){
      if(aux->dato.temp <= temp_minima) {
        temp_minima = aux->dato.temp;
        dato_ord=aux->dato;
      }
      if(aux->siguiente != NULL) {
        aux=aux->siguiente;
      } else {
        break;
      }
    }
    aux2=ELIMINAR_TEMP(aux2,temp_minima);

    lista_ord=NUEVA_TEMP(lista_ord, dato_ord);
    aux=aux2;
    if(aux==NULL){
      break;
    }
  }

  return lista_ord;
}
float CALC_MEDIANA(struct lista_temp *lista, int tamanio){
  struct lista_temp *aux, *ant;
  aux=lista;
  int contador=0;
  int valor_max=0;
  float mediana;
  valor_max=tamanio/2+1;
  //printf("valor_max=%d\n", valor_max);
  while(true) {
    contador++;
    if(contador==valor_max && tamanio%2==0){
      mediana = (aux->dato.temp + ant->dato.temp)/2;
    } else if(contador==valor_max && tamanio%2!=0){
      mediana = aux->dato.temp;
    }
    if(aux->siguiente != NULL) {
        ant=aux;
        aux=aux->siguiente;
      } else {
        break;
      }
  }
  printf("La temperatura mediana es: %.1f°C\n", mediana);
  return mediana;
}
float CALC_DESV_ESTANDAR(struct lista_temp *lista, float media, int tamanio) {
  struct lista_temp *aux;
  float desviacion=0;
  aux=lista;
  while(true){
    desviacion += pow(aux->dato.temp-media,2);
    if(aux->siguiente != NULL) {
        aux=aux->siguiente;
      } else {
        break;
      }
  }
  desviacion=desviacion/tamanio;
  desviacion=pow(desviacion,0.5);
  printf("La desviación estándar es: %.1f°C\n",desviacion);
  return desviacion;
}
void ENCONTRAR_MAX_MIN(struct lista_temp *lista){
  struct lista_temp *aux;
  aux=lista;
  struct DATO maxima, minima;
  maxima.temp = aux->dato.temp;
  minima.temp = aux->dato.temp; 
  while(true) {
    if(aux->dato.temp <= minima.temp){
      minima.temp=aux->dato.temp;
      minima.hora=aux->dato.hora;
      minima.fecha=aux->dato.fecha;
    }
    if(aux->dato.temp >= maxima.temp){
      maxima.temp=aux->dato.temp;
      maxima.hora=aux->dato.hora;
      maxima.fecha=aux->dato.fecha;
    }
    if (aux->siguiente != NULL) {
      aux=aux->siguiente;
    } 
    else {
      break;
    }
  }
  printf("La temperatura minima es: %.1f°C. Su lectura se realizó el %02d/%02d/%d a las %02d:%02d:%02d.\n",minima.temp, minima.fecha.dia, minima.fecha.mes, minima.fecha.anio, minima.hora.hora, minima.hora.minuto, minima.hora.segundo);
  printf("La temperatura máxima es: %.1f°C. Su lectura se realizó el %02d/%02d/%d a las %02d:%02d:%02d.\n",maxima.temp, maxima.fecha.dia, maxima.fecha.mes, maxima.fecha.anio, maxima.hora.hora, maxima.hora.minuto, maxima.hora.segundo);
}

struct DATO datos;
struct lista_temp *temperaturas;
FILE *archivo;

float media,mediana,moda,desviacion;
int lecturas;
char *aux, cadena[255],separador[]="|",temp_str[5],tendencia[255],hora_str[255],fecha_str[255],dia_str[255],mes_str[255],anio_str[255],horas_str[255],minutos_str[255],segundos_str[255],temp_actual;

int main(){
  temperaturas=NULL;
  archivo = fopen("Temperatura.txt","r");
  while(true) {
    aux=fgets(cadena, 28, (FILE*)archivo);
    if(aux==NULL) {
      break;
    }
    lecturas++;
    if(strncmp(cadena+4, separador, 1)==0 && strncmp(cadena+6, separador, 1)==0 && strncmp(cadena+15, separador, 1)==0) {
      // si coinciden los separadores, el formato es el que corresponde por lo tanto empieza a dividir la palabra segun corresponde a cada uno de los datos
      strncpy(temp_str, cadena, 4); 
      strncpy(tendencia, cadena+5, 1);
      strncpy(hora_str, cadena+7, 8);
      strncpy(fecha_str, cadena+16, 10);

      strncpy(horas_str, hora_str, 2);
      strncpy(minutos_str, hora_str+3, 2);
      strncpy(segundos_str, hora_str+6, 2);

      strncpy(anio_str, fecha_str, 4);
      strncpy(mes_str, fecha_str+5, 2);
      strncpy(dia_str,fecha_str+8, 2);
//pasa los datos de string a int(atoi) o float(atof) 
      datos.hora.hora= atoi(horas_str);
      datos.hora.minuto= atoi(minutos_str);
      datos.hora.segundo= atoi(segundos_str);
      datos.fecha.dia=atoi(dia_str);
      datos.fecha.mes=atoi(mes_str);
      datos.fecha.anio=atoi(anio_str);

      datos.temp=atof(temp_str);
      datos.tendencia= atoi(tendencia);

      temperaturas=NUEVA_TEMP(temperaturas, datos);
    }
    else {
      printf("Formato incorrecto.");
    }
  }
//imprime la cantidad de lecturas 
  printf("Se realizaron %d lecturas. \n",lecturas);
  printf("Lista de temperaturas: \n"); 
//inicializa todos los subprogramas que calculan cada uno de los datos
  IMPRIMIR_LISTA(temperaturas);
  ENCONTRAR_MAX_MIN(temperaturas);

  media=CALC_MEDIA(temperaturas);
  moda=CALC_MODA(temperaturas);
  desviacion=CALC_DESV_ESTANDAR(temperaturas, media, lecturas);
  mediana=CALC_MEDIANA(ordenar_lista(temperaturas), lecturas);

  return 0;
}