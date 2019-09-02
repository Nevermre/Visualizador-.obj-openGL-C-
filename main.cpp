/*

    Lucas Cunha Peres Rodrigues 83481
    Danilo Miranda Ribeiro 74251

    r - reseta a camera
    Importar os OBJ na caixa de import pelo caminho do diretório contendo o obj por exemplo C:/nomeobj.obj
    A textura que está sendo usada é a mesma para todos os três objetos pois os objetos disponibilizados
    no site não vem com arquivo bmp, está na linha 165 a abertura da textura, por favor mudar o diretorio
     para a qual está extraido o bmp

*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include<GL/freeglut.h>

#include <vector>
#include <iostream>
#include <sstream>
using namespace std;
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

GLuint jmodel;
float jmodelrot;
char ch='1';
bool leu=false;
bool luz1=true,luz2=true,luz3=true;
int index[3]={0,0,0};
int indexf[3]={0,0,0};
float inputs[3][10]={{0,0,0,0,0,0,0,1,1,1},{0,0,0,0,0,0,0,1,1,1},{0,0,0,0,0,0,0,1,1,1}};
bool inputclicado[3][10]={{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};
bool texxt=false;
bool negativo=false;
float temp[3]={0,0,0};

bool mostrar[3]={0,0,0};
bool importou1=false,caixaimportar=false;
const int font=(int)GLUT_BITMAP_HELVETICA_12;
int objimportados=0;
bool importa=false;
bool decimal=false;
GLdouble tempoAntigo, tempoNovo, variacaoDeTempo;
GLint controleFrame = 144, countFPS, countAux;
vector< string >  importar( 1 );
vector< string >  nomes( 3 );
float xi=0,yi=0;

GLuint texture[1];

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

typedef struct Image Image;

float xrot;
float yrot;
float zrot;
float ratio;

char c;


int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size; // size of the image in bytes.
    unsigned long i; // standard counter.
    unsigned short int planes; // number of planes in image (must be 1)
    unsigned short int bpp; // number of bits per pixel (must be 24)

    char temp; // temporary color storage for bgr-rgb conversion.
    // make sure the file is there.

    if ((file = fopen(filename, "rb"))==NULL){
        printf("File Not Found : %s\n",filename);
        return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }
    //printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }
    //printf("Height of %s: %lu\n", filename, image->sizeY);
    // calculate the size (assuming 24 bits or 3 bytes per pixel).

    size = image->sizeX * image->sizeY * 3;
    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }

    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }


    // read the bitsperpixel

    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        printf("Error reading bpp from %s.\n", filename);
        return 0;
    }

    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }
    // seek past the rest of the bitmap header.

    fseek(file, 24, SEEK_CUR);

    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }
    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }
    // we're done.
    return 1;
}



Image * loadTexture(){
    Image *image1;
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }

    if (!ImageLoad("C:/INF390.bmp", image1)) {
        exit(1);
    }

    return image1;
}

void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);

    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


// The limits of the viewport in pixels; this defines the drawable
//   area inside the window.


class obj{

    public:
    float x,y,z;

};
class faces{

    public:
    int v1,v2,v3;
    int t1,t2,t3;



};

class textura{
    public:
    float x,y;
};
//class face
int indext[3]={0,0,0};
std::vector <vector <obj> > lista_vertices(3,vector <obj>(150000));
std::vector <vector <faces> > lista_faces(3,vector <faces>(150000));
std::vector <vector <textura> > lista_textura(3,vector <textura>(150000));
void setViewport(int ViewportXmin, int ViewportYmin,
                 int ViewportXmax, int ViewportYmax)
{
  glViewport( ViewportXmin, ViewportYmin,   // lower left corner
              ViewportXmax - ViewportXmin,  // width
              ViewportYmax - ViewportYmin); // height
}

void loadObj(const char *fname)
{

    if(objimportados<3){
            nomes[objimportados]=fname;
        FILE *fp;
        int read;
        float x, y, z,v1,t1,n1,v2,t2,n2,v3,t3,n3;
        char ch;

        fp=fopen(fname,"r");
        if (!fp)
        {
            printf("can't open file %s\n", fname);
            exit(1);
        }
        glPointSize(2.0);

            while(!(feof(fp)))
            {

                read=fscanf(fp,"%c",&ch);
                //cout << ch << endl;
                if(ch=='v'){

                    read=fscanf(fp,"%f %f %f",&x,&y,&z);
                    if(read==3){
                     lista_vertices[objimportados][index[objimportados]].x=x;
                    lista_vertices[objimportados][index[objimportados]].y=y;
                    lista_vertices[objimportados][index[objimportados]].z=z;
                    index[objimportados]++;}



                }
                else if(ch=='f'){

                    read=fscanf(fp,"%f/%f/%f %f/%f/%f %f/%f/%f",&v1,&t1,&n1,&v2,&t2,&n2,&v3,&t3,&n3);
                    if(read==9){

                        lista_faces[objimportados][indexf[objimportados]].v1=v1;
                        lista_faces[objimportados][indexf[objimportados]].v2=v2;
                        lista_faces[objimportados][indexf[objimportados]].v3=v3;

                        lista_faces[objimportados][indexf[objimportados]].t1=t1;
                        lista_faces[objimportados][indexf[objimportados]].t2=t2;
                        lista_faces[objimportados][indexf[objimportados]].t3=t3;

                        indexf[objimportados]++;

                    }



                }
                if(ch=='t'){
                    //cout << "entrou" << endl;
                        read=fscanf(fp,"%f %f %f",&x,&y,&z);
                        if(read==3){
                            //cout << x << " " << y << endl;
                        lista_textura[objimportados][indext[objimportados]].x=x;
                        lista_textura[objimportados][indext[objimportados]].y=y;
                        //cout <<lista_textura[objimportados][indext[objimportados]].x << " " << lista_textura[objimportados][indext[objimportados]].y << endl;
                        indext[objimportados]++;
                    }
                    if(read==2){
                            //cout << x << " " << y << endl;
                        lista_textura[objimportados][indext[objimportados]].x=x;
                        lista_textura[objimportados][indext[objimportados]].y=y;
                        //cout <<lista_textura[objimportados][indext[objimportados]].x << " " << lista_textura[objimportados][indext[objimportados]].y << endl;
                        indext[objimportados]++;
                    }
                }

            }
           // cout << index;
        fclose(fp);
        objimportados++;
        leu=true;/*

        for(int j=0; j<index;j++)
            cout << lista_vertices[j].x << " " << lista_vertices[j].y << " " << lista_vertices[j].z <<endl;
            cout << lista_textura[0][j].x << " " << lista_textura[0][j].y  <<endl;
            for(int j=0; j<indext[0];j++)
    cout << "entrou" << objimportados << " " << nomes[objimportados-1] << endl;*/}

}

//funcao para desenhar um cubo colorido (desenha as suas 6 faces)
void colorcube();

//tamanho da janela em pixels
int largurajanela = 600, alturajanela = 600;
GLint  ViewportXmin = 0;
GLint  ViewportXmax = largurajanela / 3;
GLint  ViewportYmin = 0;
GLint  ViewportYmax = alturajanela / 3;



GLfloat vertices[8][3] = { {-1.0,-1.0,1.0},{-1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,-1.0,1.0},
{-1.0,-1.0,-1.0},{-1.0,1.0,-1.0},{1.0,1.0,-1.0},{1.0,-1.0,-1.0} };

//cor de cada face do cubo
GLfloat colors[6][3] = { {0.0,0.0,0.0} , {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
{0.0,0.0,1.0} , {1.0,0.0,1.0}};

//posicao do observador (camera)
GLfloat viewer[] = {2.0, 1.0, 3.0, 1.0};





// define o tipo de projecao
// glOrtho(left, right, bottom, top, near, far) -> projecao paralela
// glFrustum(left, right, bottom, top, near, far) -> projecao perspectiva
// gluPerspective(viewAngle, aspectRatio, near, far) -> projecao perspectiva
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0); // cor para limpeza do buffer
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); // projeção paralela

   glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); // projeção perspectiva

   //glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); // projeção perspectiva
   // glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 //   glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
     //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    //gluPerspective(70.0,1.0,2.0,20.0); // projeção perspectiva

glMatrixMode(GL_MODELVIEW);




   //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
}

void display(void) {
    init();

    GLint viewportWidth  = ViewportXmax - ViewportXmin;
    GLint viewportHeight = ViewportYmax - ViewportYmin;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    setViewport( 0,  0,
                  2*viewportWidth, 3*viewportHeight);

    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);

    gluLookAt(viewer[0],viewer[1],viewer[2], // define posicao do observador
    0.0, 0.0, 0.0,                           // ponto de interesse (foco)
    0.0, 1.0, 0.0);
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);


    glEnd();

    glColor3f(0,1,0);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);


    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);


    glEnd();
    glEnable(GL_TEXTURE_2D);
    if(!texxt){
    glDepthFunc(GL_LESS);
    Image *image1 = loadTexture();
    if(image1 == NULL){
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }

    // Create Texture


    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    //glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    /*

    ; //scale linearly when image bigger than texture

     //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


    */
    texxt=true;
    }

    GLfloat light0_position[ ] = { 2.0, 2.0, 0.0, 1.0 };
    GLfloat light0_ambient [] = {0.3, 0.3, 0.3, 1.0};
    GLfloat light0_diffuse [] = {0.3, 0.3, 0.3, 1.0};
    GLfloat light0_specular [] = {0.3, 0.3, 0.3, 1.0};

    GLfloat light1_position[ ] = { 0.0, 2.0, 0.0, 1.0 };
    GLfloat light1_ambient [] = {0.1, 0.1, 0.1, 1.0};
    GLfloat light1_diffuse [] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light1_specular [] = {1.0, 1.0, 1.0, 1.0};

    GLfloat light2_ambient [] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light2_diffuse [] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light2_specular [] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light2_position [] = {viewer[0], viewer[1], viewer[2], 1.0};
    GLfloat spot_direction [] = {0.0,0.0,-1.0};




    GLfloat teto[] = { 0.0, 2.0, 0.0, 1.0 };
    GLfloat white_light[ ] = { 1.0, 1.0, 1.0, 0.0 };

    GLfloat mat_ambient[ ] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat mat_diffuse[ ] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[ ] = { 0.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[ ] = { 1.0, 1.0, 1.0, 1.0 };


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    //glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2f );
     glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    glLightfv (GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv (GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
    glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1.0);

    glLightfv (GL_LIGHT2, GL_AMBIENT, light2_ambient);
    glLightfv (GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv (GL_LIGHT2, GL_SPECULAR, light2_specular);
    glLightfv (GL_LIGHT2, GL_POSITION, viewer);
    //glLightf (GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.0);
    glLightf (GL_LIGHT2, GL_LINEAR_ATTENUATION, 1.0);
    //glLightf (GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0);
    glLightf (GL_LIGHT2, GL_SPOT_CUTOFF, 20.0);
    glLightfv (GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf (GL_LIGHT2, GL_SPOT_EXPONENT, 30.0);


     glEnable(GL_LIGHTING);
    if(luz1==true)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);
    if(luz2==true)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
    if(luz3==true)
        glEnable(GL_LIGHT2);
    else
        glDisable(GL_LIGHT2);


    glEnable(GL_COLOR_MATERIAL);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);



    int l=0;
    while(l<objimportados){
            glPushMatrix();
            glTranslatef(inputs[l][0],inputs[l][1],inputs[l][2]);
            glRotatef(inputs[l][3],inputs[l][4],inputs[l][5],inputs[l][6]);
    glScalef(inputs[l][7],inputs[l][8],inputs[l][9]);
     for(int k=0; k<indexf[l];k++){
         if(l==0)
            glColor3f(0.5,0.5,0.5);
        else if(l==1)
            glColor3f(0.5,0.5,0.5);
        else
            glColor3f(0.5,0.5,0.5);


            int soma;
            if(lista_faces[l][k].v1<0)
                       soma=index[l];
            else soma=0;

            int soma2;
            if(lista_faces[l][k].t1<0)
                soma2=indext[l];
            else soma2=0;
        if(mostrar[l]==0){
        glBegin(GL_POLYGON);
           glTexCoord2f(lista_textura[l][soma2+lista_faces[l]
                       [k].t1].x, lista_textura[l][soma2+lista_faces[l]
                       [k].t1].y);
            glVertex3f( lista_vertices[l][soma+lista_faces[l]
                       [k].v1].x,lista_vertices[l][soma+lista_faces[l]
                       [k].v1].y,lista_vertices[l][soma+lista_faces[l]
                       [k].v1].z );

            glTexCoord2f(lista_textura[l][soma2+lista_faces[l]
                       [k].t2].x, lista_textura[l][soma2+lista_faces[l]
                       [k].t2].y);
                       glVertex3f( lista_vertices[l][soma+lista_faces[l]
                       [k].v2].x,lista_vertices[l][soma+lista_faces[l]
                       [k].v2].y,lista_vertices[l][soma+lista_faces[l]
                       [k].v2].z );

            glTexCoord2f(lista_textura[l][soma2+lista_faces[l]
                       [k].t3].x, lista_textura[l][soma2+lista_faces[l]
                       [k].t3].y);
                       glVertex3f( lista_vertices[l][soma+lista_faces[l]
                       [k].v3].x,lista_vertices[l][soma+lista_faces[l]
                       [k].v3].y,lista_vertices[l][soma+lista_faces[l]
                       [k].v3].z );



        glEnd(); } }
        l++;glPopMatrix();}
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glColor4f(1,1,1,0.5);
        glBegin(GL_QUADS);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
	glEnd();
    glPopMatrix();

    glDisable(GL_BLEND);

                         // vetor de "view up"
    //glFlush();
   // colorcube(); //desenha o cubo
   setViewport( 2*viewportWidth,  0,
                  3*viewportWidth, 3*viewportHeight);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

    gluOrtho2D(-10,10,-10,10);
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_LIGHTING);

    glColor3f (0.3, 0.0, 0.3);
    glBegin(GL_POLYGON);

        glVertex2f(-10,-10);
        glVertex2f(10,-10);
        glVertex2f(10,10);
        glVertex2f(-10,10);


    glEnd();

    glColor3f (1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);

        glVertex2f(-9.8,9);
        glVertex2f(5,9);
        glVertex2f(5,9.8);
        glVertex2f(-9.8,9.8);


    glEnd();
    glColor3f (0.6, 0.6, 0.6);
    glBegin(GL_POLYGON);

        glVertex2f(5,9);
        glVertex2f(10,9);
        glVertex2f(10,9.8);
        glVertex2f(5,9.8);


    glEnd();
    glColor3f (0.0, 0.0, 0.0);
    renderBitmapString(5.5,9.2,(void *)font,"Importar");

    glColor3f(0.0,0.0,0.0);

        renderBitmapString(-9.5,9.2,(void *)font,importar[0].c_str());

    if(objimportados>0){
        renderBitmapString(-9.8,8.0,(void *)font,nomes[0].c_str());
        renderBitmapString(-9.8,7.0,(void *)font,"Translacao");
        renderBitmapString(-9.8,6.0,(void *)font,"Rotacao");
        renderBitmapString(-9.8,5.0,(void *)font,"Escala");

        if(!mostrar[0])
            glColor3f(1.0,1.0,1.0);
        else
            glColor3f(0.0,1.0,0.0);

        glBegin(GL_POLYGON);

                    glVertex2f(0.0 ,8.0);
                    glVertex2f(2,8);
                    glVertex2f(2,8.5);
                    glVertex2f(0,8.5);


                glEnd();
         glColor3f(1.0,1.0,1.0);
        for(int q=0;q<3;q++){
            if(q==1){
                glBegin(GL_POLYGON);

                    glVertex2f(-5.0 ,7.0-q);
                    glVertex2f(-3,7-q);
                    glVertex2f(-3,7.5-q);
                    glVertex2f(-5,7.5-q);


                glEnd();


            }
            for(int r=0;r<3;r++){

                glBegin(GL_POLYGON);

                    glVertex2f(-2.0 + r*4,7.0-q);
                    glVertex2f(1+r*4,7-q);
                    glVertex2f(1+r*4,7.5-q);
                    glVertex2f(-2+r*4,7.5-q);


                glEnd();

            }
        }


    glColor3f(0.0,0.0,0.0);
        for(int r=0;r<10;r++){
            std::stringstream ss; //from
            ss << inputs[0][r];
            std::string s = ss.str ();
            char* char_type = (char*) s.c_str();
            if(r<3 &&r>=0)
                renderBitmapString(-1+r*4,7,(void *)font,char_type);
            else if(r==3)
                renderBitmapString(-4,6,(void *)font,char_type);
            else if(r<=6 &&r>3)
                renderBitmapString(-1+(r-4)*4,6,(void *)font,char_type);
            else
                renderBitmapString(-1+(r-7)*4,5,(void *)font,char_type);

        }

    }

    if(objimportados>1){
        glColor3f(0.0,0.0,0.0);
        renderBitmapString(-9.8,3.0,(void *)font,nomes[1].c_str());
        renderBitmapString(-9.8,2.0,(void *)font,"Translacao");
        renderBitmapString(-9.8,1.0,(void *)font,"Rotacao");
        renderBitmapString(-9.8,0.0,(void *)font,"Escala");

        if(!mostrar[1])
            glColor3f(1.0,1.0,1.0);
        else
            glColor3f(0.0,1.0,0.0);
         glBegin(GL_POLYGON);

                    glVertex2f(0.0 ,3.0);
                    glVertex2f(2,3);
                    glVertex2f(2,3.5);
                    glVertex2f(0,3.5);


                glEnd();
         glColor3f(1.0,1.0,1.0);
        for(int q=0;q<3;q++){
            if(q==1){
                glBegin(GL_POLYGON);

                    glVertex2f(-5.0 ,2.0-q);
                    glVertex2f(-3,2-q);
                    glVertex2f(-3,2.5-q);
                    glVertex2f(-5,2.5-q);


                glEnd();


            }
            for(int r=0;r<3;r++){

                glBegin(GL_POLYGON);

                    glVertex2f(-2.0 + r*4,2.0-q);
                    glVertex2f(1+r*4,2-q);
                    glVertex2f(1+r*4,2.5-q);
                    glVertex2f(-2+r*4,2.5-q);


                glEnd();

            }
        }
        glColor3f(0.0,0.0,0.0);
        for(int r=0;r<10;r++){
            std::stringstream ss; //from
            ss << inputs[1][r];
            std::string s = ss.str ();
            char* char_type = (char*) s.c_str();
            if(r<3 &&r>=0)
                renderBitmapString(-1+r*4,2,(void *)font,char_type);
            else if(r==3)
                renderBitmapString(-4,1,(void *)font,char_type);
            else if(r<=6 &&r>3)
                renderBitmapString(-1+(r-4)*4,1,(void *)font,char_type);
            else
                renderBitmapString(-1+(r-7)*4,0,(void *)font,char_type);

        }



    }

    if(objimportados>2){
        glColor3f(0.0,0.0,0.0);
        renderBitmapString(-9.8,-2.0,(void *)font,nomes[2].c_str());
        renderBitmapString(-9.8,-3.0,(void *)font,"Translacao");
        renderBitmapString(-9.8,-4.0,(void *)font,"Rotacao");
        renderBitmapString(-9.8,-5.0,(void *)font,"Escala");

        if(!mostrar[2])
            glColor3f(1.0,1.0,1.0);
        else
            glColor3f(0.0,1.0,0.0);
         glBegin(GL_POLYGON);

                    glVertex2f(0.0 ,-1.5);
                    glVertex2f(2,-1.5);
                    glVertex2f(2,-2.0);
                    glVertex2f(0,-2.0);


                glEnd();
         glColor3f(1.0,1.0,1.0);
        for(int q=0;q<3;q++){
            if(q==1){
                glBegin(GL_POLYGON);

                    glVertex2f(-5.0 ,-2.5-q);
                    glVertex2f(-3,-2.5-q);
                    glVertex2f(-3,-3.0-q);
                    glVertex2f(-5,-3.0-q);


                glEnd();


            }
            for(int r=0;r<3;r++){

                glBegin(GL_POLYGON);

                    glVertex2f(-2.0 + r*4,-2.5-q);
                    glVertex2f(1+r*4,-2.5-q);
                    glVertex2f(1+r*4,-3.0-q);
                    glVertex2f(-2+r*4,-3.0-q);


                glEnd();

            }
        }
     glColor3f(0.0,0.0,0.0);
        for(int r=0;r<10;r++){
            std::stringstream ss; //from
            ss << inputs[2][r];
            std::string s = ss.str ();
            char* char_type = (char*) s.c_str();
            if(r<3 &&r>=0)
                renderBitmapString(-1+r*4,-3.0,(void *)font,char_type);
            else if(r==3)
                renderBitmapString(-4,-4.0,(void *)font,char_type);
            else if(r<=6 &&r>3)
                renderBitmapString(-1+(r-4)*4,-4.0,(void *)font,char_type);
            else
                renderBitmapString(-1+(r-7)*4,-5.0,(void *)font,char_type);

        }


    }

    if(luz1==true)
        glColor3f(1.0,1.0,1.0);
    else
        glColor3f(0.5,0.5,0.5);

    glBegin(GL_POLYGON);

                    glVertex2f(-9 ,-8);
                    glVertex2f(-7,-8);
                    glVertex2f(-7,-7.5);
                    glVertex2f(-9,-7.5);


    glEnd();

    if(luz2==true)
        glColor3f(1.0,1.0,1.0);
    else
        glColor3f(0.5,0.5,0.5);

    glBegin(GL_POLYGON);

                    glVertex2f(-6 ,-8);
                    glVertex2f(-4,-8);
                    glVertex2f(-4,-7.5);
                    glVertex2f(-6,-7.5);


    glEnd();

    if(luz3==true)
        glColor3f(1.0,1.0,1.0);
    else
        glColor3f(0.5,0.5,0.5);

    glBegin(GL_POLYGON);

                    glVertex2f(-3 ,-8);
                    glVertex2f(-1,-8);
                    glVertex2f(-1,-7.5);
                    glVertex2f(-3,-7.5);


    glEnd();

    glColor3f(1.0,1.0,1.0);

    std::stringstream ss; //from
    ss << (indexf[0]+indexf[1]+indexf[2]);
    std::string s = ss.str ();
    char* char_type = (char*) s.c_str();
    renderBitmapString(-9,-9,(void*)font,"Triangulos: ");
    renderBitmapString(-3,-9,(void*)font, char_type);

    int num=100*(1000.0/countFPS);
    ss.str("");
    ss << (1.0*num)/100.0 ;
     s = ss.str ();
    char_type = (char*) s.c_str();
    renderBitmapString(4,-9,(void*)font, char_type);
    renderBitmapString(6.5,-9,(void*)font, "MS");

    glFlush();
 //   glutSwapBuffers(); //usando double buffer (para evitar 'flicker')
 glutPostRedisplay();

}

void mousemotion(int x, int y ){

    if(x-xi>0){
        viewer[0]+=0.05;
        //viewer[2]-=0.05;
    }

    else if(x-xi<0){
        viewer[0]-=0.05;
        //viewer[2]+=0.05;
    }

    if(y-yi>0){
        viewer[1]+=0.05;
        viewer[2]-=0.05;
    }

   else  if(y-yi<0){
        viewer[1]-=0.05;
        viewer[2]+=0.05;
   }


}

void HandleMouse(int button, int state, int x, int y)
{

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){

        cout << "x = " << x << "  y = " << y << endl;
        if(x>400 && x<545 && y<30 && y>5)
            caixaimportar=true;
        else
            caixaimportar=false;
        if(x >500 && x<520){
            if(y>45 && y<60)
                mostrar[0]=( (mostrar[0] == 0 )? 1 :0 );
            if(y>195 && y<210)
                mostrar[1]=( (mostrar[1] == 0) ? 1 :0) ;
            if(y>345 && y<360)
                mostrar[2]=( (mostrar[2] == 0 )? 1 :0) ;
        }

        if(x>550 && x<600 && y>5 && y<30){
            loadObj(importar[0].c_str());
            caixaimportar=false;
        }
        for(int p=0;p<3;p++)
            for(int w=0;w<10;w++)
                inputclicado[p][w] = 0;

        if(y>75 && y<90){

            if(x>480 && x<510)
                inputclicado[0][0]=1;


            if(x>520 && x<550)
                inputclicado[0][1]=1;

            if(x>560 && x<590)
                inputclicado[0][2]=1;

        }
        else if(y>105 && y<120){
            if(x>450 && x<470)
                inputclicado[0][3]=1;

            if(x>480 && x<510)
                inputclicado[0][4]=1;


            if(x>520 && x<550)
                inputclicado[0][5]=1;

            if(x>560 && x<590)
                inputclicado[0][6]=1;



        }
        else if(y>135 && y<150){


            if(x>480 && x<510)
                inputclicado[0][7]=1;


            if(x>520 && x<550)
                inputclicado[0][8]=1;

            if(x>560 && x<590)
                inputclicado[0][9]=1;



        }
        if(y>225 && y<240){

            if(x>480 && x<510)
                inputclicado[1][0]=1;


            if(x>520 && x<550)
                inputclicado[1][1]=1;

            if(x>560 && x<590)
                inputclicado[1][2]=1;

        }
        else if(y>255 && y<270){
            if(x>450 && x<470)
                inputclicado[1][3]=1;

            if(x>480 && x<510)
                inputclicado[1][4]=1;


            if(x>520 && x<550)
                inputclicado[1][5]=1;

            if(x>560 && x<590)
                inputclicado[1][6]=1;



        }
        else if(y>285 && y<300){


            if(x>480 && x<510)
                inputclicado[1][7]=1;


            if(x>520 && x<550)
                inputclicado[1][8]=1;

            if(x>560 && x<590)
                inputclicado[1][9]=1;



        }
        if(y>375 && y<390){

            if(x>480 && x<510)
                inputclicado[2][0]=1;


            if(x>520 && x<550)
                inputclicado[2][1]=1;

            if(x>560 && x<590)
                inputclicado[2][2]=1;

        }
        else if(y>405 && y<420){
            if(x>450 && x<470)
                inputclicado[2][3]=1;

            if(x>480 && x<510)
                inputclicado[2][4]=1;


            if(x>520 && x<550)
                inputclicado[2][5]=1;

            if(x>560 && x<590)
                inputclicado[2][6]=1;



        }
        else if(y>435 && y<450){


            if(x>480 && x<510)
                inputclicado[2][7]=1;


            if(x>520 && x<550)
                inputclicado[2][8]=1;

            if(x>560 && x<590)
                inputclicado[2][9]=1;



        }
        else if(y>525 && y<540){
            if(x>410 && x<430)
                luz1= (luz1==0) ? 1 : 0;
            if(x>440 && x<460)
                luz2= (luz2==0) ? 1 : 0;
            if(x>470 && x<490)
                luz3= (luz3==0) ? 1 : 0;
        }
        else{

            xi=x;
            yi=y;
        }


    }

    negativo=false;

}

void keyboard(unsigned char key, int x, int y) {
    if(caixaimportar){
        if( key == 8 ){
            if(!importar.back().empty() )
                importar.back().erase(importar.back().size()-1 );
        }
        else
            importar.back().push_back( key );



    }
    if(!caixaimportar){

            for(int p=0;p<3;p++)
                for(int w=0;w<10;w++){
                    if(inputclicado[p][w]==1){
                            int z=0;
                            if(key=='1')
                                z=1;
                            if(key=='2')
                                z=2;
                            if(key=='3')
                                z=3;
                            if(key=='4')
                                z=4;
                            if(key=='5')
                                z=5;
                            if(key=='6')
                                z=6;
                            if(key=='7')
                                z=7;
                            if(key=='8')
                                z=8;
                            if(key=='9')
                                z=9;
                            if(key=='-'){
                                inputs[p][w]=-1*(inputs[p][w]);
                                negativo=true;
                            }
                        else{
                            if( key == 8){
                                z=inputs[p][w]/10;
                                inputs[p][w]=z;
                            }
                        else{
                            if(key=='.')
                                decimal=true;

                        else{
                            if(!decimal){
                                if(negativo)
                                    inputs[p][w]=10*inputs[p][w]-z;
                                else
                                inputs[p][w]=10*inputs[p][w]+z;
                            }
                            else{
                                float aux=z;
                                if(negativo)
                                    inputs[p][w]=inputs[p][w]-aux/10;
                                else
                                inputs[p][w]=inputs[p][w]+aux/10;
                                decimal=false;

                            }
                        }}}
                        cout << inputs[p][w] << endl;

                    }

                }





    }
    if (key == 27) exit(0); //ESC

    if (key == 'd') viewer[0] += 0.5;
    if (key == 'a') viewer[0] -= 0.5;
    if (key == 'w')

        viewer[2]-=0.5;

            //viewer[2] -= 1.0;}
    if (key == 's') viewer[2] += 0.5;
    if(key=='r'){
        viewer[0] = 2.0;
        viewer[1] = 1.0;
        viewer[2] = 3.0;
        cout << "aa" << endl;

    }
    display();
}


int main(int argc, char **argv) {
                                                                                    //loadObj("C:/CornellBox-Sphere.obj");
    glutInit(&argc,argv); //inicializa a glut
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH ); //tipo de buffer/cores
    glutInitWindowSize(largurajanela, alturajanela); //dimensões da janela
    glutInitWindowPosition(100, 100); //posicao da janela
    glutCreateWindow("Terceiro Trabalho - INF390 "); //cria a janela
    init();

    glutDisplayFunc(display); //determina função callback de desenho
    glutKeyboardFunc(keyboard); //determina função callback de teclado
    glutMouseFunc(HandleMouse);
    glutMotionFunc(mousemotion);

    glEnable(GL_DEPTH_TEST); //habilita remoção de superfícies ocultas usando Z-Buffer


 //   glEnable (GL_CULL_FACE); //habilita remoção de superfícies ocultas usando Back Facxe Culling
 //   glCullFace(GL_FRONT);    // Remove as faces da frente
 //   glCullFace(GL_BACK);    // Remove as faces de tras
    tempoAntigo = glutGet(GLUT_ELAPSED_TIME);
	double teste = glutGet(GLUT_ELAPSED_TIME);
    int countAux = 0;
	while(true){

		tempoNovo = glutGet(GLUT_ELAPSED_TIME);
		variacaoDeTempo = tempoNovo - tempoAntigo;
		if(controleFrame == 0 || variacaoDeTempo >= ( 1000 / controleFrame )){
			tempoAntigo = tempoNovo;
			glutMainLoopEvent();
			countAux++;
		}
		if((glutGet(GLUT_ELAPSED_TIME) - teste) >= 1000){
			countFPS = countAux;
			countAux = 0;
			teste = glutGet(GLUT_ELAPSED_TIME);
		}
	}

    //glutMainLoop();
    return 0;
}
