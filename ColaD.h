#ifndef COLADP_H
#define COLADP_H

struct nodocp{
	int dato;
	struct nodocp * psig;
};
typedef struct nodocp NODOCP;

struct colap{
	NODOCP * pa_delantera;
	NODOCP * pa_trasera;
};
typedef struct colap STCOLAP;

void crear_colap(STCOLAP *qp)
{
	qp->pa_delantera = NULL;
	qp->pa_trasera = NULL;
}

int colap_vacia(STCOLAP qp)
{
	return qp.pa_delantera == NULL;
}

int insertar(STCOLAP *qp, int dat)
{
	NODOCP * nuevonodo,*temp;

	nuevonodo = (NODOCP *) malloc(sizeof(NODOCP));
	if(nuevonodo == NULL)
		return 0;
	nuevonodo->dato = dat;
	nuevonodo->psig = NULL;
	if(colap_vacia(*qp))
		qp->pa_delantera = nuevonodo;
	else{
		  qp->pa_trasera->psig = nuevonodo;		
        }
             
	qp->pa_trasera = nuevonodo;

	return 1;
}

int remover(STCOLAP *qp, int *dat)
{
	NODOCP *temp, *ganon, *sombra;

	if(colap_vacia(*qp))
		return 0;
		
	temp = qp->pa_delantera;
    *dat = temp->dato;
    qp->pa_delantera = qp->pa_delantera->psig;
    
    if( qp->pa_delantera == NULL ){
       qp->pa_trasera=NULL; 
    }
    	
	return 1;
}

int vaciar_colap(STCOLAP *qp)
{
	int dat;
	
	if(colap_vacia(*qp))
		return 0;
	while(!colap_vacia(*qp)){
		remover(qp, &dat);
	}
	return 1;
}

#ifdef APIWIN32

void trazar_tierra(HDC hdc, int x, int y)
{
	int i;

	for(i = 0; i < 3; i++){
		MoveToEx(hdc, x-(10-i*3), y+i*5, NULL);
		LineTo(hdc, x+(10-i*3), y+i*5);
	}
}

void trazar_puntero(HDC hdc, int x, int y, int l, int f)
{
	Ellipse(hdc, x, y, x+10, y+10);
	MoveToEx(hdc, x+10, y+5, NULL);
	LineTo(hdc, x+l, y+5);
	if(f){
		LineTo(hdc, x+l-5, y);
		MoveToEx(hdc, x+l, y+5, NULL);
		LineTo(hdc, x+l-5, y+10);
	}
}


void trazar_nodocp(HDC hdc, NODOCP nodo, int x, int y)
{
	TCHAR szDato[16];

	Rectangle(hdc, x, y, x+50, y+25);
	Rectangle(hdc, x+50, y, x+75, y+25);
	Rectangle(hdc, x+75, y, x+100, y+25);
	TextOut(hdc, x+3, y+3,szDato, sprintf(szDato,"%d",nodo.dato));
	TextOut(hdc, x+55, y+3, szDato, sprintf(szDato,"%d",nodo.prioridad));
	trazar_puntero(hdc, x+85, y+7, 25, TRUE);
}

int trazar_colap(HDC hdc, STCOLAP qp, int x, int y)
{
	NODOCP *temp;
	int xtemp;

	Rectangle(hdc,x,y,x+50, y+25);
	Rectangle(hdc,x,y+25,x+50, y+50);
	Rectangle(hdc,x,y+50,x+50, y+75);
	trazar_puntero(hdc, x+30, y+7, 30, TRUE);
	if(colap_vacia(qp)){
		trazar_tierra(hdc, x+70, y+15);
		trazar_puntero(hdc, x+30, y+32, 30, TRUE);
		trazar_tierra(hdc, x+70, y+42);
		return 0;
	}

	xtemp = x+60;
	temp = qp.pa_delantera;
	while(temp != NULL){
		trazar_nodocp(hdc, *temp, xtemp, y);
		xtemp+=110;
		temp =  temp->psig;
	}
	trazar_tierra(hdc, xtemp+10, y+15);
	xtemp-=110;
	trazar_puntero(hdc, x+30, y+32, xtemp-x,FALSE);
	MoveToEx(hdc,xtemp+30,y+37,NULL);
	LineTo(hdc,xtemp+30,y+25);
	LineTo(hdc,xtemp+25,y+30);
	MoveToEx(hdc,xtemp+30,y+25,NULL);
	LineTo(hdc,xtemp+35,y+30);

	return 1;
}

#endif
#endif
