#define LIBERADO 0
#define PRESIONADO 1

struct stCuadrito{
  RECT rect;
  HPEN hpen; 
  HBRUSH brochas[2];		
  int estado;
  int ID;
};

typedef struct stCuadrito STCUADRITO;

void crear_cuadrito(STCUADRITO *c)
{
    c->rect.top=0;
    c->rect.left=0;
    c->rect.bottom=0;
    c->rect.right=0;
    c->brochas[0]=NULL;
    c->brochas[1]=NULL;
    c->hpen=NULL;
    c->estado = LIBERADO;            
}

int trazar_cuadrito(STCUADRITO c, HDC hdc)
{
    HPEN hpant;
    
    if(c.hpen==NULL)
    {
        MessageBox(NULL,"No hay lapiz","Error",MB_OK|MB_ICONERROR);
        return 0;
    }
    hpant = SelectObject(hdc,c.hpen);
    Rectangle(hdc,c.rect.left,c.rect.top,c.rect.right,c.rect.bottom);
    SelectObject(hdc,hpant);
    
    if(c.estado == LIBERADO)
    {
        FillRect(hdc,&c.rect,c.brochas[0]);
    }else{
            FillRect(hdc,&c.rect,c.brochas[1]);
        }        
    
    return 1;
}
int en_cuadrito(STCUADRITO *c, int x, int y,int *posicion)
{
   int i;
    
   for(i=0;i<9;i++)
   {
      if( x>=c[i].rect.left && x<=c[i].rect.right && y>=c[i].rect.top && y<=c[i].rect.bottom)
    {
        *posicion = c[i].ID;
        return 1;
    }   
   }    
    return 0;
}
