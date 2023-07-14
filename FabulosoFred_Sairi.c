#include <windows.h>
#include "Constantes.h"
#include "Cuadro.h"
#include "ColaD.h"


//Declaracion de funciones.
void CrearCuadros(STCUADRITO *cuadro); //Crea los botones
void Tablero(STCUADRITO *cuadro, HDC hdc,int num,HWND hwnd,int TURNO); //Genera el tablero
void ColorRelleno( HBRUSH brochas[][2] ); //Pinta los botones
int GenerarAleatorio(); //Genera la secuencia aleatoria.
int Reiniciar(HWND hwnd,HWND hBtnIniciar,HWND hBtnSalir,STCOLAP *cola1,STCOLAP *cola2,
            int *px,int *py,int *estado,int *posicion,int *nivel,int *TURNO); //Reinicia todo una vez concluido el juego.
void AsignarAtributos(STCUADRITO *cuadro, HDC hdc, HPEN hpen, HBRUSH brochas[][2]);
// Se encarga de todo lo relacionado con el tipo de fuente y de dibujar las cadenas.
void EscribirTexto(HDC hDC, HWND hWnd, char mensaje[], int x, int y);  
void DibujarchCadena(HDC hDC, HWND hWnd, char chCadena[], int x, int y); 

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;        
      
    static HINSTANCE hInstance;    

    static HWND hBtnIniciar,hBtnSalir,hEdtNivel;
    
    int j,i,num;
    static int px,py,estado,posicion,TURNO,nivel;       
    
    static STCOLAP cola1,cola2;
    static STCUADRITO cuadro[9];
    static HBRUSH brochas[9][2];    
    static HPEN hpen;    
    static RECT rect[2];
      
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:{
            
            CrearCuadros( cuadro );
			hpen = GetStockObject( BLACK_PEN );               
			ColorRelleno( brochas );       
            AsignarAtributos( cuadro , hdc , hpen , brochas );                      			                                  
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;   
                     
            
            srand( time(NULL) );       
            px=py=0;
            estado = EDONORMAL;
            posicion=-1;            
            nivel = 0;   
            crear_colap(&cola1);
            crear_colap(&cola2);            
            TURNO = INACTIVO;
            
            rect[0].left = 100;
            rect[0].top = 460;
            rect[0].right = 550;
            rect[0].bottom = 505;
            
            rect[1].left = 490;
            rect[1].top = 350;
            rect[1].right = 565;
            rect[1].bottom = 395;
            
            break;
            }
        case WM_LBUTTONDOWN: {
            px = LOWORD(lParam);
            py = HIWORD(lParam);
            
            if( estado == EDOESPERA )
            {
                if( en_cuadrito(cuadro,px,py,&posicion) )
                { 
                               
                               remover(&cola2,&num);
                               insertar(&cola1,num);
                               cuadro[posicion].estado = PRESIONADO;
                               InvalidateRect(hwnd,&cuadro[posicion].rect, TRUE);
                               estado = EDOPARPADEO;                         
                               SetTimer(hwnd,IDT_TIMER,500,NULL);                                                                                  
                }
            }                    
            break;    
        }             
        case WM_TIMER:{
            
            switch(wParam){
  
                case IDT_TIMER:{
               
                     switch(TURNO){
                        
                            case MAQUINA:{
                                
                                switch(estado){
                                    
                                    case EDONORMAL:{
                                
                                           remover(&cola1,&posicion);
                                           insertar(&cola2,posicion);                                           
                                           cuadro[posicion].estado = PRESIONADO;
                                           InvalidateRect(hwnd,&cuadro[posicion].rect, TRUE);
                                           estado = EDOPARPADEO;
                                     break;   
                                    }
                                    case EDOPARPADEO:{
                                            cuadro[posicion].estado = LIBERADO;
                                            InvalidateRect(hwnd,&cuadro[posicion].rect, TRUE);   
                                                 
                                                                            
                                            if( colap_vacia(cola1) )
                                            {
                                                  KillTimer(hwnd,IDT_TIMER);                        
                                                  estado = EDOESPERA;  
                                                  TURNO =USUARIO;
                                                  InvalidateRect(hwnd,&rect[0],TRUE);
                                                 
                                                                                          
                                            }else{
                                                    estado = EDONORMAL;   
                                                 }                              
                                     break;    
                                    }    
                                }
                             break;   
                            }       
                            case USUARIO:{
                                
                                switch(estado){
                                                                                                
                                   case EDOPARPADEO:{
                                            cuadro[posicion].estado = LIBERADO;
                                            InvalidateRect(hwnd,&cuadro[posicion].rect, TRUE);  
											Beep(600,200);                                       
                                    
                                                                                  
                                            if( cuadro[posicion].ID == cola1.pa_trasera->dato )
                                             {
                                               
                                                if(colap_vacia(cola2) )
                                                {                                              
                                                  estado = EDONORMAL;                                            
                                                  TURNO = MAQUINA;
                                                  nivel++;        
                                                  InvalidateRect(hwnd,&rect[0],TRUE);
                                                  InvalidateRect(hwnd,&rect[1],TRUE);                                                                                                                                         
                                                  
                                                  num = GenerarAleatorio();                                           
                                                                                         
                                                  insertar(&cola1,num);          
                                                     
                                                }else{
                                                        KillTimer(hwnd,IDT_TIMER);
                                                        estado = EDOESPERA;   
                                                     }                
                                             }else{
                                                    KillTimer(hwnd,IDT_TIMER);                                                    
                                                    Reiniciar(hwnd,hBtnIniciar,hBtnSalir,&cola1,&cola2,
                                                    &px,&py,&estado,&posicion,&nivel,&TURNO);
                                                    InvalidateRect(hwnd,&rect[0],TRUE);
                                                    InvalidateRect(hwnd,&rect[1],TRUE);
                                                  }                               
                                     break;
                                   }
                                }
                             break;       
                            }
                     }                                                                                                                                                                                                                          
                  break;
                }    
            }
          break;  
        }
                       
        case WM_COMMAND:{
            int wmId    = LOWORD(wParam);
		    int wmEvent = HIWORD(wParam);
		  // Analizar las selecciones de comandos:
    		switch (wmId)
    		{
    			case IDM_SALIR:{
					DestroyWindow(hwnd);
					break;
				}
				case IDM_ACERCA:{
					MessageBox(hwnd,"Creadora: Hernández Avila Jaina Sairi\nCorreo: sairihdza@gmail.com\nGrupo: 204",
					"Acerca de", MB_OK|MB_ICONASTERISK);
					break;
				}
					case IDM_INS:{
					MessageBox(hwnd,"Da clic en Archivo, Nuevo Juego.\nEl juego generará una secuencia aleatoria que pintará los cuadros en blanco, procura seguirla dando clic en los cuadros una vez que cambien de color, de lo contrario perderás.",
					"Instrucciones", MB_OK|MB_ICONASTERISK);
					break;
				}
				case IDM_INICIAR:{                                          
                        if(estado == EDONORMAL){
						
                            num = GenerarAleatorio()-1;
                          
                            insertar(&cola1,num);
                            TURNO = MAQUINA;
                            nivel++;       
                        InvalidateRect(hwnd,&rect[0],TRUE);
                        InvalidateRect(hwnd,&rect[1],TRUE);
                        SetTimer(hwnd,IDT_TIMER,500,NULL);
                    }
                    break;
    			}
    	   			
    			default:
    				return DefWindowProc(hwnd, message, wParam, lParam);		
             }            
            break;
            }                                    
        case WM_PAINT:{
            hdc = BeginPaint(hwnd,&ps);           
            
            Tablero(cuadro,hdc,nivel,hwnd,TURNO);                            
            
            EndPaint(hwnd,&ps);
            break;
            }                        
        case WM_DESTROY:

            DeleteObject(hpen); 	       
            
			for(i=0;i<9;i++)		
            {
				for(j=0;j<2;j++){
					DeleteObject(brochas[i][j]);
				}
            }           
            
            vaciar_colap(&cola1);
            vaciar_colap(&cola2);

            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void Tablero(STCUADRITO *cuadro, HDC hdc,int num,HWND hwnd,int TURNO)
{    
    HBRUSH hbrTemp;
    int i;  
    char chCadena[100]="Nivel ";       
    
    hbrTemp = GetStockObject(WHITE_BRUSH);
    Rectangle(hdc,80,80,420,420); 
    SelectObject(hdc,hbrTemp); 
    DeleteObject(hbrTemp);                     
    
   for(i=0;i<9;i++)
   { 
      trazar_cuadrito(cuadro[i],hdc);
   }
   //NIVEL
    DibujarchCadena( hdc, hwnd, chCadena, 470, 300 );
   //CONTEO DEL NIVEL
    sprintf(chCadena,"%d",num);
    DibujarchCadena( hdc, hwnd, chCadena, 510, 340 ); 
    
    switch(TURNO){
        case MAQUINA:{                  
              sprintf(chCadena,"%s","Turno Máquina");
              DibujarchCadena( hdc, hwnd, chCadena, 130, 430 );
              
        break; 
        }    
        case USUARIO:{
              sprintf(chCadena,"%s","Turno Usuario");
              DibujarchCadena( hdc, hwnd, chCadena, 130, 430 );
        break; 
        }    
        case INACTIVO:{
              sprintf(chCadena,"%s","");
              DibujarchCadena( hdc, hwnd, chCadena, 130, 430 );
        break; 
        }    
    }
    
}

void CrearCuadros(STCUADRITO *cuadro)
{
    int i;
   //Se crean los botones.
    for(i=0;i<9;i++)
    {
        crear_cuadrito( &cuadro[i] );
    }            
}            

void ColorRelleno( HBRUSH brochas[][2] )
{
	//Colores de los cuadritos.
		brochas[0][0] = CreateSolidBrush(RGB(243,12,12));
		brochas[1][0] = CreateSolidBrush(RGB(243,125,12));
		brochas[2][0] = CreateSolidBrush(RGB(252,247,16));
		brochas[3][0] = CreateSolidBrush(RGB(30,225,36));			
		brochas[4][0] = CreateSolidBrush(RGB(30,218,225));	
		brochas[5][0] = CreateSolidBrush(RGB(30,48,225));	
		brochas[6][0] = CreateSolidBrush(RGB(141,101,196));
		brochas[7][0] = CreateSolidBrush(RGB(204,0,102));
		brochas[8][0] = CreateSolidBrush(RGB(240,127,144));
			
	//Parpadeo blanco
		brochas[0][1] = CreateSolidBrush(RGB(255,255,255));			
		brochas[1][1] = CreateSolidBrush(RGB(255,255,255));			
		brochas[2][1] = CreateSolidBrush(RGB(255,255,255));						
		brochas[3][1] = CreateSolidBrush(RGB(255,255,255));		
		brochas[4][1] = CreateSolidBrush(RGB(255,255,255));	
		brochas[5][1] = CreateSolidBrush(RGB(255,255,255));		
		brochas[6][1] = CreateSolidBrush(RGB(255,255,255));
		brochas[7][1] = CreateSolidBrush(RGB(255,255,255));
		brochas[8][1] = CreateSolidBrush(RGB(255,255,255));
}

void AsignarAtributos(STCUADRITO *cuadro, HDC hdc, HPEN hpen, HBRUSH brochas[][2])
{
    int j,k,i;
    
    for(j=0,k=0;j<3;j++){					
		
        for(i=0;i<3;i++)
        {                      
            cuadro[k+i].rect.top = 100+(100*j);
            cuadro[k+i].rect.left = 100+(100*i);
            cuadro[k+i].rect.right = 100+(i+1)*100;
            cuadro[k+i].rect.bottom = 100+(j+1)*100;   
            cuadro[k+i].brochas[0]=brochas[k+i][0];
            cuadro[k+i].brochas[1]=brochas[k+i][1];
            cuadro[k+i].hpen = hpen;
            cuadro[k+i].ID = k+i;
        }
      k+=i;
    }
}
int GenerarAleatorio()
{
    int num=0;    
    return num = rand() % 9;
}

int Reiniciar(HWND hwnd,HWND hBtnIniciar,HWND hBtnSalir,STCOLAP *cola1,STCOLAP *cola2,
                int *px,int *py,int *estado,int *posicion,int *nivel,int *TURNO)
{
    int num=0;
    
    if( IDCANCEL == MessageBox (hwnd,"Usted ha perdido","GAME OVER",MB_ICONERROR))
     {
        DestroyWindow(hwnd);    
     }   
                            
    while(!colap_vacia(*cola1)){ remover(cola1,&num); }
    
    while(!colap_vacia(*cola2)){ remover(cola2,&num); }
     
    *px=*py=0;
    *estado = EDONORMAL;
    *posicion=-1;       
    *nivel = 0;
    *TURNO = INACTIVO;
    
}
void EscribirTexto(HDC hDC, HWND hWnd, char mensaje[], int x, int y) {
   HFONT fuente, anterior;
   COLORREF color;
   LOGFONT lf= {40, 0, 0, 0, 300, FALSE, FALSE, FALSE,
              DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
              PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN, "KG Life Is Messy"};

   fuente = CreateFontIndirect(&lf);

   SetBkMode(hDC, TRANSPARENT);

   anterior = SelectObject(hDC, fuente);
   color = SetTextColor(hDC, RGB(255,255,255));
   
   TextOut(hDC, x, y, mensaje, strlen(mensaje));

   SetTextColor(hDC, color);
   SelectObject(hDC, anterior);
   DeleteObject(fuente);
}
void DibujarchCadena(HDC hDC, HWND hWnd, char chCadena[], int x, int y) {
	int i = 0;
	char car[8];
	
	while(chCadena[i] != '\0') {
		car[0] = chCadena[i];
		car[1] = '\0';
		EscribirTexto(hDC,hWnd,car, x+20*i, y+20);
		i++;
	}
}         

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "MAINMENU";                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW+3);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Fabuloso Fred",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           600,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


