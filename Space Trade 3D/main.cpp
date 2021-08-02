#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/glu.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "helpFile.hpp"

using namespace sf;

class nni
{
public:
	int INT;                     //Not
	std::string STRING;          //Needed
	char CHAR;                   //Information

	bool clear()
	{
		INT = 0;
		STRING = "";
		CHAR = 32;
	}
};
nni NNI;

float angleX, angleY; // Углы поворота камеры
//float size = 20.0f;


class obj3dmodel
{
	struct vertex {
		double x;
		double y;
		double z;
	};
	struct vn {
		double x;
		double y;
		double z;
	};
	struct vt {
		double x;
		double y;
	};
	struct face {
		unsigned int v1, v2, v3, v4;
		unsigned int vn1, vn2, vn3, vn4;
		unsigned int vt1, vt2, vt3, vt4;
	};
	std::vector<vertex> vetexes;
	std::vector<float> points;
	std::vector<vn> vn;
	std::vector<vt> vt;
	std::vector<face> faces;
	std::vector<GLuint> verfaces;

public:
	float x = 0, y = 0, z = 0;
	float scaleX = 5, scaleY = 5, scaleZ = 5;

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
				this->points.push_back(v.x);
				this->points.push_back(v.y);
				this->points.push_back(v.z);
			}
			//break;
			case 'f':
			{
				face f;

				std::string tmp1, tmp2, tmp3, tmp4;
				fin >> tmp1 >> tmp2 >> tmp3 >> tmp4;

			int pos = tmp1.find("/"); f.v1  = int(tmp1.substr(0, pos).c_str()); tmp1.erase(0, pos + 1);
				pos = tmp1.find("/"); f.vt1 = int(tmp1.substr(0, pos).c_str()); tmp1.erase(0, pos + 1);
				f.vn1 = int(tmp1.c_str());
				pos = tmp2.find("/"); f.v2  = int(tmp2.substr(0, pos).c_str()); tmp2.erase(0, pos + 1);
				pos = tmp2.find("/"); f.vt2 = int(tmp2.substr(0, pos).c_str()); tmp2.erase(0, pos + 1);
				f.vn2 = int(tmp2.c_str());
				pos = tmp3.find("/"); f.v3  = int(tmp3.substr(0, pos).c_str()); tmp3.erase(0, pos + 1);
				pos = tmp3.find("/"); f.vt3 = int(tmp3.substr(0, pos).c_str()); tmp3.erase(0, pos + 1);
				f.vn3 = int(tmp3.c_str());
				pos = tmp4.find("/"); f.v4  = int(tmp4.substr(0, pos).c_str()); tmp4.erase(0, pos + 1);
				pos = tmp4.find("/"); f.vt4 = int(tmp4.substr(0, pos).c_str()); tmp4.erase(0, pos + 1);
				f.vn4 = int(tmp4.c_str());

				faces.push_back(f);
				verfaces.push_back(f.v1);
				verfaces.push_back(f.v2);
				verfaces.push_back(f.v3);
				verfaces.push_back(f.v4);
			}
			break;
			}
		}
	}

	void old_draw()
	{
		//glColor3ub(145, 30, 66);
		glBegin(GL_POLYGON);
		for (int i = 0; i < faces.size(); i++)
		{
			//glColor3f(0.1, 0.2, 0.3);

			vertex v1 = vetexes[faces[i].v1 - 1];
			vertex v2 = vetexes[faces[i].v2 - 1];
			vertex v3 = vetexes[faces[i].v3 - 1];
			//vertex v4 = vetexes[faces[i].v4 - 1];

			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(v1.x * scaleX, v1.y * scaleY, v1.z * scaleZ);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(v2.x * scaleX, v2.y * scaleY, v2.z * scaleZ);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(v3.x * scaleX, v3.y * scaleY, v3.z * scaleZ);
			//glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(v4.x * scaleX, v4.y * scaleY, v4.z * scaleZ);
		}
		glEnd();
	}

	void draw()
	{
		glVertexPointer(3, GL_FLOAT, 0, &points);
		glEnableClientState(GL_VERTEX_ARRAY);
			glColor3f(1.0f, 0.0f, 0.0f);
			glDrawElements(GL_POLYGON, verfaces.size(), GL_UNSIGNED_INT, &verfaces);
		glDisableClientState(GL_VERTEX_ARRAY);
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
	RenderWindow window(VideoMode(800, 600), "Space Trade 3D");
	
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 2000.f);  
    glEnable(GL_TEXTURE_2D);

	ShowCursor(FALSE);

	obj3dmodel model;
	model.readfile("cat.obj");
	model.scaleX = 50;
	model.scaleY = 50;
	model.scaleZ = 50;

	////карта высот////
	/*Image im;  im.loadFromFile("resources/heightmap.png");

	for(int x=0;x<256;x++)
 	 for(int z=0;z<256;z++)
	 {
		 int c = im.getPixel(x,z).r / 15;
         for(int y=0;y<c;y++)
	       if (y>c-3) mass[x][y][z]=1;
	 }*/
	 


    ///////текстуры///////
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

	//////основной цикл/////////
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

angleX +=(xt - mousexy.x)/4; //4 — чувствительность 
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
		    while(dist<120)  // радиус действия
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

     
//////////рисуем боксы///////
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

	   glTranslatef(0, 0, -10);
	   createBox(box, 20);
	   glTranslatef(0, 0, 10);

	   glTranslatef(0, 0, 10);
	   model.draw();
	   glTranslatef(0, 0, -10);


		window.pushGLStates();
		window.draw(s);      //рисуем курсор
        window.popGLStates();
		
        window.display();
    }

    return 0;
}
