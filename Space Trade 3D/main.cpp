#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/glu.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "helpFile.hpp"

using namespace sf;

float angleX, angleY; // ���� �������� ������
//float size = 20.0f;


class obj3dmodel
{
	struct vertex {
		double x;
		double y;
		double z;
	};
	struct face {
		unsigned int v1, v2, v3;
	};
	std::vector<vertex> vetexes;
	std::vector<face> faces;

public:
	float x = 0, y = 0, z = 0;
	float scaleX = 1, scaleY = 1, scaleZ = 1;

	void readfile(const char* filename)
	{
		std::string s;
		std::ifstream fin(filename);
		if (!fin)
			return;
		while (fin >> s)
		{
			switch (*s.c_str())
			{
			case 'v':
			{
				vertex v;
				fin >> v.x >> v.y >> v.z;
				this->vetexes.push_back(v);
			}
			break;
			case 'f':
			{
				face f;
				fin >> f.v1 >> f.v2 >> f.v3;
				faces.push_back(f);
			}
			break;
			}
		}
	}

	void draw()
	{
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < faces.size(); i++)
		{
			vertex v1 = vetexes[faces[i].v1 - 1];
			vertex v2 = vetexes[faces[i].v2 - 1];
			vertex v3 = vetexes[faces[i].v3 - 1];

			glVertex3f(v1.x * scaleX, v1.y * scaleY, v1.z * scaleZ);
			glVertex3f(v2.x * scaleX, v2.y * scaleY, v2.z * scaleZ);
			glVertex3f(v3.x * scaleX, v3.y * scaleY, v3.z * scaleZ);
		}
		glEnd();
	}
};


class Player
{ 
public:
   float x,y,z;
   float dx,dy,dz;
   float w,h,d;  // width, height, depth 
   bool onGround;
   float speed;

   Player(float x0, float y0, float z0)
   {
    x=x0; y=y0; z=z0;
	dx=0; dy=0; dz=0;
	w=5; h=20; d=5; speed=5;
	onGround=false;
   }

  void update(float time)
   {
     //if (!onGround) dy-=1.5*time;
	 //onGround=0;
	 
	 x+=dx*time;
     collision(dx,0,0);	     
     y+=dy*time;        
     collision(0,dy,0);
     z+=dz*time;
     collision(0,0,dz);
 
	 dx=dz=0; 
   }

  void collision(float Dx,float Dy,float Dz)
  {
    for (int X=(x-w)/size;X<(x+w)/size;X++)
    for (int Y=(y-h)/size;Y<(y+h)/size;Y++)
    for (int Z=(z-d)/size;Z<(z+d)/size;Z++)
      if (check(X,Y,Z))  {
        if (Dx>0)  x = X*size-w; 
        if (Dx<0)  x = X*size+size+w; 
		if (Dy>0)  y = Y*size-h; 
        if (Dy<0) {y = Y*size+size+h; onGround=true; dy=0;} 
		if (Dz>0)  z = Z*size-d; 
        if (Dz<0)  z = Z*size+size+d; 
                          }
  }

  void keyboard()
  {
		if (Keyboard::isKeyPressed(Keyboard::Space)) if (onGround) {onGround=false; dy=12;};

		if (Keyboard::isKeyPressed(Keyboard::W))	
		{
		 dx =- sin(angleX/180*PI) * speed; 
		 dz =- cos(angleX/180*PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::S))	
		{
		 dx = sin(angleX/180*PI) * speed;
		 dz = cos(angleX/180*PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::D))		
		{
		 dx = sin((angleX+90)/180*PI) * speed;	
		 dz = cos((angleX+90)/180*PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::A))	
		{
		 dx = sin((angleX-90)/180*PI) * speed;	
		 dz = cos((angleX-90)/180*PI) * speed;
		}
  }

};

int main()
{
	//size = 20.0f;
	RenderWindow window(VideoMode(800, 600), "Minecraft C++");
	
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 2000.f);  
    glEnable(GL_TEXTURE_2D);

	ShowCursor(FALSE);

	obj3dmodel model;
	model.readfile("example.obj");
	model.scaleX = 50;
	model.scaleY = 50;
	model.scaleZ = 50;

	////����� �����////
	Image im;  im.loadFromFile("resources/heightmap.png");

	for(int x=0;x<256;x++)
 	 for(int z=0;z<256;z++)
	 {
		 int c = im.getPixel(x,z).r / 15;
         for(int y=0;y<c;y++)
	       if (y>c-3) mass[x][y][z]=1;
	 }
	 


    ///////��������///////
	Texture t; t.loadFromFile("resources/cursor.png");
	Sprite s(t); s.setOrigin(8,8); s.setPosition(400,300); 
	
	
	  GLuint skybox[6];
	skybox[0] = LoadTexture("resources/skybox/skybox_front.bmp");
	skybox[1] = LoadTexture("resources/skybox/skybox_back.bmp");
	skybox[2] = LoadTexture("resources/skybox/skybox_left.bmp");
    skybox[3] = LoadTexture("resources/skybox/skybox_right.bmp");
	skybox[4] = LoadTexture("resources/skybox/skybox_bottom.bmp");
	skybox[5] = LoadTexture("resources/skybox/skybox_top.bmp");


      GLuint box[6];
	box[0] = LoadTexture("resources/grassBox/side.jpg");
	box[1] = LoadTexture("resources/grassBox/side.jpg");
	box[2] = LoadTexture("resources/grassBox/side.jpg");
	box[3] = LoadTexture("resources/grassBox/side.jpg");
	box[4] = LoadTexture("resources/grassBox/bottom.jpg");
	box[5] = LoadTexture("resources/grassBox/top.jpg");
	//////////////////////////



	Clock clock;
	bool mLeft=0,mRight=0; // mouse buttons
    Player p(-100,0,0);

	//////�������� ����/////////
	while (window.isOpen())
    {
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		time = time/50;
		if (time>3) time=3;


        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape))
                window.close();

			if (event.type == Event::MouseButtonPressed)
				{if (event.key.code == Mouse::Right) mRight=1;
				 if (event.key.code == Mouse::Left)  mLeft=1;}
        }


glClear(GL_DEPTH_BUFFER_BIT);

p.keyboard();
p.update(time);


////-----------------------
POINT mousexy;
GetCursorPos(&mousexy);
int xt=window.getPosition().x+400; 
int yt=window.getPosition().y+300;

angleX +=(xt - mousexy.x)/4; //4 � ���������������� 
angleY +=(yt - mousexy.y)/4;

if (angleY<-89.0){angleY=-89.0;}
if (angleY>89.0){angleY=89.0;}

SetCursorPos(xt,yt);       
////----------------------



        if (mRight || mLeft)
          {
			float x = p.x;
			float y = p.y+p.h/2;
            float z = p.z;

			int X,Y,Z,oldX,oldY,oldZ;
			int dist=0;
		    while(dist<120)  // ������ ��������
		    {
		      dist++;

			 x += -sin(angleX/180*PI);    X=x/size;
             y +=  tan(angleY/180*PI);    Y=y/size;
             z += -cos(angleX/180*PI);    Z=z/size;
			
		     if (check(X,Y,Z))
			   if (mLeft) { mass[X][Y][Z]=0; break;}
			   else   {mass[oldX][oldY][oldZ]=1; break;}
			   
			 oldX=X; oldY=Y; oldZ=Z;
			}
	      }
  
mLeft=mRight=0;

        
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(p.x,p.y+p.h/2,p.z,p.x-sin(angleX/180*PI),p.y+p.h/2+(tan(angleY/180*PI)),p.z-cos(angleX/180*PI), 0, 1, 0); 

     
//////////������ �����///////
   int R = 30;  
		
   int X=p.x/size;
   int Y=p.y/size;
   int Z=p.z/size;
	
    for(int x=X-R; x<X+R; x++)
 	 for(int y=0;y<25;y++)
 	  for(int z=Z-R;z<Z+R;z++)
	  {
		  if (!check(x,y,z)) continue;

		   glTranslatef(size*x+size/2, size*y+size/2, size*z+size/2);

		   createBox(box, size/2);

  		   glTranslatef(-size*x-size/2, -size*y-size/2, -size*z-size/2);
	  }
       

	    
	   glTranslatef(p.x, p.y, p.z);
	   createBox(skybox,1000);
	   glTranslatef(-p.x, -p.y, -p.z);

	   glTranslatef(10, 10, 10);
	   createBox(box, 20);
	   glTranslatef(-10, -10, -10);

	   glTranslatef(30, 30, 30);
	   model.draw();
	   glTranslatef(-30, -30, -30);


		window.pushGLStates();
		window.draw(s);      //������ ������
        window.popGLStates();
		
        window.display();
    }

    return 0;
}