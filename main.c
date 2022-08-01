#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include <string.h>
#include <GRAPHICS.H>
#include <defined.h>
#include <abstract.h>

/*
tamano_fijo
 Establecer los identificadores de cada una de las figuras disponibles del programa */

enum TIPO_FIG {
    CIRCULO_F = 4,
    RECTANGULO_F = 3,
    TRIANGULO_F = 2,
    LINEA_F = 1
};

/*  Propiedades de una Figura.*/
typedef struct EstructuraFig {
    Point origen;
    int tamano;
    enum TIPO_FIG tipo;
    enum COLORS color;
} Figura;

/* Iniciar toda la logica principal del programa */
void InitPrograma(Lienzo *, enum COLORS, enum COLORS);

/* Iniciar menus */
void menu_helipses(Lienzo *, Ellipse (*)[], const char *[]);
void menu_figuras(Lienzo *board, Rectangle (*)[], enum COLORS);
void menu_colores(Lienzo *, Rectangle (*)[], int);

/* mostrar las helipses*/
void mostrar(Ellipse *, enum COLORS, const char *, enum COLORS);

/* logica acerca de como posicionar una figura sellecionada */
int figura_seleccionada(Figura **, int, Cursor *);
Rectangle *obtener_dimensiones_figura(Rectangle *, Figura *);
int posicionar_figura(Lienzo *, Figura **, int, enum TIPO_FIG, enum COLORS);

void borrar();


void dibujar_figura(Figura *, enum COLORS);
void barra(Lienzo *, enum COLORS);

/* Determinar si el puntero esta dentro de alguna figura en relacion a sus dimensiones */
int dentro_figura(Figura *, Point *);

int ejecutar(void);

/*Ejecutar programa*/
int main() {return ejecutar();}



void InitPrograma(Lienzo *lienzo, enum COLORS foreground_colour, enum COLORS fondo_color) {

    Point limites_motor = *init_graphics_engine(&limites_motor);
    (*lienzo) = *generate_board(lienzo, &limites_motor, 5, 8, 15, 15);

    /* Dibujar el lienzo */
    dibujar_lienzo(lienzo, WHITE);

    /* Generar desborde de colores para pintar las partes necesarias */
    setfillstyle(SOLID_FILL, fondo_color);
    floodfill(lienzo->limit.x / 2, lienzo->area.start.y / 2, WHITE);
}

void menu_helipses(Lienzo *lienzo, Ellipse (*helipses)[], const char *aristas[]) {
    int i = 0;
    double ancho = lienzo->width;
    double altura = lienzo->limit.y - lienzo->area.end.y;
    int ancho_fijo = (int) (ancho / 3) - (int) ((int) (ancho / 3) * .10);
    for (; i < 3; i++) {
        Point centro;
        centro.x = lienzo->area.start.x / 2 + measure_center(ancho, i + 1, 3, .10);
        centro.y = (int) (lienzo->area.end.y + (altura / 2));
        mostrar(generate_ellipse(&(*helipses)[i], &centro, ancho_fijo, (int) altura, 30, 70), BLACK, aristas[i], WHITE);
    }
}

void menu_figuras(Lienzo *lienzo, Rectangle (*opciones)[], enum COLORS color) {
    int i = 0;
    const int tamano_fijo = (int) ((int) (lienzo->width / 4) -
                                  (int) (lienzo->width / 4) * .1);
    const enum COLORS color_anterior = getcolor();
    struct linesettingstype linea_anterior;
    getlinesettings(&linea_anterior);
    for (; i < 4; i++) {

        const enum TIPO_FIG tipo = i + 1;
        Point centro;
        double ancho = lienzo->limit.x - lienzo->area.end.x,
                altura = (int) (lienzo->area.start.y / 2) + measure_center(lienzo->height, i + 1, 4, .1);
        centro.x = (int) (lienzo->area.end.x + (ancho / 2));
        centro.y = (int) altura;
        setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
        generate_rectangle(&(*opciones)[i], &centro, tamano_fijo, tamano_fijo, 0);

        if (tipo == CIRCULO_F) {
            Circle circulo = *generate_circle(&circulo, &centro, tamano_fijo / 4);
			      draw_circle(&circulo, color);

        } else if (tipo == RECTANGULO_F) {
            Rectangle rectangulo = *generate_rectangle(&rectangulo, &centro, tamano_fijo / 2, tamano_fijo / 2, 0);
            draw_rectangle(&rectangulo, color);
            setfillstyle(SOLID_FILL, color);
            bar((&rectangulo)->start.x, (&rectangulo)->start.y, (&rectangulo)->end.x, (&rectangulo)->end.y);

        } else if (tipo == TRIANGULO_F) {
            Triangle triangulo = *generate_triangle(&triangulo, &centro, tamano_fijo / 2);
            draw_triangle(&triangulo, color);

        } else if (tipo == LINEA_F) {
            setcolor(color);
            line(centro.x - tamano_fijo / 4, centro.y, centro.x + tamano_fijo / 4, centro.y);

        }
    }
    setcolor(color_anterior);
    setlinestyle(linea_anterior.linestyle, (int) linea_anterior.upattern, linea_anterior.thickness);
}



void menu_colores(Lienzo *board, Rectangle (*colours)[], int size) {
    int index = 0;
    struct linesettingstype old_line;
    getlinesettings(&old_line);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    for (; index < size; index++) {
        Point center;
        const int fixed_height = (int) ((int) (board->height / size) -
                                        (int) (board->height / size) * .1);
        double width = board->area.start.x, height =
                board->area.start.y + 20 + measure_center(board->height, index + 1, size, .1);
        setfillstyle(SOLID_FILL, index);
        center.x = (int) (width / 2);
        center.y = (int) height;
        center.y = (int) height;

        draw_mini_circle(generate_rectangle(&(*colours)[index], &center, (int) width - (int) (width * .15), fixed_height, 0),
        BLACK);

    }
    setlinestyle(old_line.linestyle, (int) old_line.upattern, old_line.thickness);

    /* restaurar rectangulos de referencia para los botones de colores */
    index = 0;
    for (; index < size; index++) {
        Point center;
        const int fixed_height = (int) ((int) (board->height / size) -
                                        (int) (board->height / size) * .1);
        double width = board->area.start.x, height =
                board->area.start.y + 20 + measure_center(board->height, index + 1, size, .1);
        setfillstyle(SOLID_FILL, index);
        center.x = (int) (width / 2);
        center.y = (int) height;
        center.y = (int) height;

        generate_rectangle(&(*colours)[index], &center, (int) width - (int) (width * .15), fixed_height, 1);
        /*floodfill(center.x, center.y, BLACK);*/
    }
}

void mostrar(Ellipse *ellipse, enum COLORS colour, const char *text, enum COLORS text_colour) {
    draw_ellipse(ellipse, colour);
    setfillstyle(SOLID_FILL, colour);
    floodfill(ellipse->center.x, ellipse->center.y, colour);
    text_center(ellipse->center.x, ellipse->center.y, text, text_colour);
}

void hide(Ellipse *ellipse, const char *text) {
    mostrar(ellipse, DARKGRAY, text, DARKGRAY);
}

int figura_seleccionada(Figura **figures, int count, Cursor *click) {
    int index = 0;
    for (; index < count; index++)
        if (dentro_figura(&(*figures)[index], &click->end)) return index;
    return -1;
}

int posicionar_figura(Lienzo *board, Figura **figures, int figures_count, enum TIPO_FIG type, enum COLORS colour) {
    Figura figure;
    const int figure_id = figures_count - 1;
    figure.tipo = type;
    figure.color = colour;
    figure.origen = board->center;
    figure.tamano = 65;
    /* Add more space before utilizing it */
    (*figures) = (Figura *) realloc(figures, sizeof(Figura) * (figures_count + 1));
    (*figures)[figure_id] = figure;
	  dibujar_figura(&(*figures)[figure_id], (*figures)[figure_id].color);
    ++figures_count;
    return figure_id;
}

void dibujar_figura(Figura *figure, enum active_colour colour) {
    const enum COLORS old_color = getcolor();
    struct linesettingstype old_line;
    getlinesettings(&old_line);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    setcolor(colour);
    if (figure->tipo == RECTANGULO_F) {
        setfillstyle(SOLID_FILL, colour);
        bar(figure->origen.x - figure->tamano / 2, figure->origen.y - figure->tamano / 2,
                  figure->origen.x + figure->tamano / 2, figure->origen.y + figure->tamano / 2);
        /*Borde*/
        setcolor(BLACK);
        setfillstyle(SOLID_FILL, BLACK);
        rectangle(figure->origen.x - figure->tamano / 2, figure->origen.y - figure->tamano / 2,
            figure->origen.x + figure->tamano / 2, figure->origen.y + figure->tamano / 2);

    } else if (figure->tipo == CIRCULO_F) {
		circle(figure->origen.x, figure->origen.y, figure->tamano / 2);
        floodfill(figure->origen.x, figure->origen.y, colour);

        /*Borde*/
        setcolor(BLACK);
		circle(figure->origen.x, figure->origen.y, figure->tamano / 2);

    } else if (figure->tipo == TRIANGULO_F) {

        line(figure->origen.x - figure->tamano / 2, figure->origen.y + figure->tamano / 2, figure->origen.x,
             figure->origen.y - figure->tamano / 2);
        line(figure->origen.x, figure->origen.y - figure->tamano / 2, figure->origen.x + figure->tamano / 2,
             figure->origen.y + figure->tamano / 2);
        line(figure->origen.x - figure->tamano / 2, figure->origen.y + figure->tamano / 2,
             figure->origen.x + figure->tamano / 2, figure->origen.y + figure->tamano / 2);

    } else if (figure->tipo == LINEA_F) {
        line(figure->origen.x - figure->tamano / 2, figure->origen.y, figure->origen.x + figure->tamano / 2,
             figure->origen.y);
    }
    setlinestyle(old_line.linestyle, (int) old_line.upattern, old_line.thickness);
    setcolor(old_color);
}

void borrar() {

  setfillstyle(SOLID_FILL, WHITE);
  bar(51, 23, 493+51, 385+23);
}

int dentro_figura(Figura *fig, Point *point) {
    Rectangle figure;
    return is_in_rectangle(obtener_dimensiones_figura(&figure, fig), point);
}

Rectangle *obtener_dimensiones_figura(Rectangle *dimension, Figura *figure) {
    dimension->start.x = figure->origen.x - figure->tamano / 2;
    dimension->start.y = figure->origen.y - figure->tamano / 2;
    dimension->end.x = figure->origen.x + figure->tamano / 2;
    dimension->end.y = figure->origen.y + figure->tamano / 2;
    return dimension;
}

void barra(Lienzo *board, enum COLORS colour) {
    const int x = board->area.start.x / 2, y = (board->limit.y - board->area.start.y) / 2;
    Point center = *generate_point(&center, x, y);
    Rectangle rectangle = *generate_rectangle(&rectangle, &center, (int) (board->area.start.x * .85), (int) ((board->limit.y - board->area.start.y) * .85), 0);
    draw_rectangle(&rectangle, WHITE);
    setfillstyle(SOLID_FILL, colour);
    floodfill(board->area.start.x / 2, (int) ((board->limit.y - board->area.start.y) * .85), BLACK);
}

int ejecutar() {

  const char *etiquetas[] = {"Cortar", "Borrar", "Salir"};

  Lienzo board;
  Cursor click;
  Ellipse ellipses[3];
  Rectangle figure_options[4];
  Rectangle colours[MAXCOLORS + 1];
  int figures_count = 1;
  Figura *figures = (Figura *) calloc(figures_count, sizeof(Figura));
  int active_figure = -1, active_figure_option = -1, active_menu_option = -1;
  int cortar, cortar_activado;
  enum COLORS active_colour = LIGHTRED;
  const enum COLORS foreground_colour = LIGHTGRAY, background_colour = BLUE;

  InitPrograma(&board, foreground_colour, background_colour);
  menu_figuras(&board, &figure_options, active_colour);
  menu_helipses(&board, &ellipses, etiquetas);
  menu_colores(&board, &colours, MAXCOLORS + 1);
  barra(&board, active_colour);
  mver();
  msituar(1, board.center.x, board.center.y);

  click.type = CURSOR_MOUSE; /* Make sure we're using the mouse cursor */

  cortar = 0;
  cortar_activado = 0;


  while (1) {

      const enum CLICK_TYPES click_type = click.button;
      int locked = lock_click(&board, digest_click(&click));


      /* Semantica sobre el exterior (controla toda la logica de lo que esta fuera del lienzo) */
      if (!locked) {
          /* Determines if the cursor is currently positioned on a colour option */
          int selected_colour = selected_mini_circle(&colours, MAXCOLORS + 1, &click.end);
          /* Determines if the cursor is currently positioned on an ellipse menu option */
          int selected_menu = selected_ellipse(&ellipses, 3, &click.end);
          /* Determines if the cursor is currently positioned on a figure option */
          int selected_figure = selected_rectangle(&figure_options, 4, &click.end);
          /* Handle previous click event */

          if (click_type == CLICK_LEFT) {
              /* Handle actions that are only valid until the event has been stopped, specifically outside the drawing area */
              if (click.button == CLICK_NONE && !click.held) {
                  /* Handle ellipse menu buttons triggers if the click started inside itself */
                  /*Aqui se valida el click en el menu de las helipses*/
                  if (selected_menu != -1 && is_in_ellipse(&ellipses[selected_menu], &click.start)) {
                      /* menu is array-index based, 0 being the first element in the array */
                      if (selected_menu == 0) {
                        if (cortar_activado) {
                          cortar = 0;
                          cortar_activado = 0;
                        } else {
                          cortar = 1;
                          cortar_activado = 1;
                        }
                      }

                      if (selected_menu == 1) {
                        borrar();
                        active_figure = -1;
                      }

                      if (selected_menu == 2) break;
                  }
                      /* Handle change of selected_colour event */
                      /*Aqui se valida el click en el menu de los colores*/
                  else if (selected_colour != -1 && is_in_rectangle(&colours[selected_colour], &click.start)) {
                      active_colour = selected_colour;
                      menu_figuras(&board, &figure_options, active_colour);
                      /* Changes the active selected figure's colour if any */
                      if (active_figure != -1) {
                          dibujar_figura(&figures[active_figure], active_colour);
						              figures[active_figure].color = active_colour;
                      }
                  }
                      /* Handle figure selection option if the click started inside the option */
                      /*Aqui se valida el click en el menu de las figuras*/
                  else if (selected_figure != -1 && is_in_rectangle(&(figure_options)[selected_figure], &click.start)) {
                      active_figure = posicionar_figura(&board, &figures, figures_count, selected_figure + 1,
                                                   active_colour);
                  }
              }
          }
          /* Handle hover effects over button while mouse is only moving around */
          if (click.button == CLICK_NONE) {
              if (click_type == CLICK_NONE) {
                  /* handle mouse hover event */

                  if (selected_menu != -1 && selected_menu != active_menu_option) {

                      draw_ellipse_border(&ellipses[selected_menu], YELLOW);
                    } else if (active_menu_option != -1 && selected_menu != active_menu_option) {
                      draw_ellipse_border(&ellipses[active_menu_option], BLUE);
                    }
                  active_figure_option = selected_figure;
                  active_menu_option = selected_menu;
              }
          }
      } else {
        /* Semantica sobre el interior (controla toda la logica de lo que esta dentro del lienzo) */
          if (click_type == CLICK_LEFT) { /* Este if valida si se realizo un click izquierdo dentro del lienzo */

               if (click.button == CLICK_NONE && !click.held) { /* Este if valida que el click haya sido soltado, todas las acciones solo se ejecutan cuando el click fue presionado pero ya ha sido soltado */

                  if (active_figure != -1 && !dentro_figura(&figures[active_figure], &click.start)) {

                    /* active_figure = -1;
                       esta semantica cuando active_figure != 1 es verdad valida como verdadero que el click
                       esta fuera de la figura activa y establece active_figure != 1 como falso.
                    */

                  } else if (active_figure != -1 && dentro_figura(&figures[active_figure], &click.start)) {

                      Rectangle figure = *obtener_dimensiones_figura(&figure, &figures[active_figure]);

                      if (cortar) {
                        copy_section(&figure.start, &figure.end, &click.end, COPY_PUT);
                      }

                      figures[active_figure].origen = click.end;
                      dibujar_figura(&figures[active_figure], active_colour);
                      figure = *obtener_dimensiones_figura(&figure, &figures[active_figure]);

                  } else {

                    /*Estas sentencias solo se ejecutan si no hay una figura activa aun es decir active_figure == 1 es verdad*/

                      int selected_figure = figura_seleccionada(&figures, figures_count, &click);

                      if (selected_figure != -1 && selected_figure != active_figure &&

                          dentro_figura(&figures[selected_figure], &click.start)) {
                          if (active_figure != -1)
                          active_figure = selected_figure;

                      }

                  }

              }
          }
          else if (click_type == CLICK_NONE && click.button == CLICK_NONE) {
              /* handle mouse hover event */
              if (active_figure_option != -1)
                  /*draw_rectangle_border(&figure_options[active_figure_option], active_colour);*/
              if (active_menu_option != -1)
                  draw_ellipse_border(&ellipses[active_menu_option], YELLOW);
              active_figure_option = active_menu_option = -1;
          }
      }
  }
  free(figures);
  close_graphics_engine();
  return 0;
}
