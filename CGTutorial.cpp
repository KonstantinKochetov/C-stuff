// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// opengl libraries
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;// import package

// Achtung, die OpenGL-Tutorials nutzen glfw 2.7, glfw kommt mit einem ver�nderten API schon in der Version 3 

// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"

// Erkl�ren ObenGL-Statemachine, lowlevel
// Version 1: seit 1992, elegantes API f�r die Plattformunabh�gige 3D-Programmierung 
// Version 2: seit 2002, Erg�nzung durch Shader-Programme, die auf Grafikkarte laufen
// Version 3: seit 2008, Fixedfunction-Pipeline nur noch als Compability-Extension
// Version 4: seit 2010 noch kaum unterst�tzt
// Wir setzen nun auf Version 3, schwieriger zu erlernen als Version 1
// glm-Bibliothek f�r Matrix-Operationen (fehlen in Version3), glew-Bibliothek, um API-Funktionen > 1.1 nutzen zu k�nnen
// glfw-Bibliothek, um OpenGL plattformunabh�ngig nutzen zu k�nnen (z. B. Fenster �ffnen)
#define UEBUNG1 /* feste Rotation */
// Pflichtteil: Ergebnis zeigen ...
// Kode zeilenweise erkl�ren. (Orientiert sich an http://www.opengl-tutorial.org/, Evtl. diese Tutorials dort ausf�hren, um zu vertiefen.)
// Dort wird glfw in der Version 2 genutzt, wir verwenden die aktuelle Version 3 http://www.glfw.org/
// Schwarze Linien wg. Shader. Kurz erklaeren, was die Shader machen...
// Vorgehensweise erklaeren, Jeweils alte cpp-Datei nach CGTutorial.cpp.n kopieren, damit jede Aenderung nachvollziehbar bleibt.
// (Voraussetzung fuer Support)
// Aufgabe Rotation: glm::rotate    (http://glm.g-truc.net/glm.pdf)
#define UEBUNG2 /* variable Rotation ueber R-Taste */
// Eventloop, kann man muss man aber nicht in glfw (Alternativ Glut, dort muss man)
// Aufgabe: Hinweis globale Variable, Taste...
#define UEBUNG3 /* Kamerasteuerung */
// Aufgabe drei Unabh�ngige Rotationen, wird zu erstem Teil des Pflichtteils
// Hinweis auf andere Kaperasteuerungen ->  http://www.opengl-tutorial.org
#define UEBUNG4 /* Ausgemaltes Objekt und Z-Buffer */
// OpenGL-Befehle: http://wiki.delphigl.com/index.php/Hauptseite auf Deutsch!
// GLEW http://glew.sourceforge.net/
// Wireframe vs. Solid thematisieren, Z-Buffer wird noetig, um Verdeckungsproblem zu l�sen
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
///////////////////////////////////////////////////////
#define UEBUNG5 /* Einlesen eines Objekts */
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
// Bessere OBJ-Loader vorhanden, basieren aber auf OpenGL1.1
// Andere Formate m�glich, z. B. 3ds, s. a. Link am Ende der Seite
// Wir nehmen Teapot, Anekdote zu Teapot:  von Newell 1975 (hat ber�hmtes Lehrbuch geschrieben) glut, 3dsmax, Toystory, Monster AG, ...
//
// Vertex Buffer Objects (VBO) (ab 1.5) enthalten alle Daten zu Oberflaechen eines Objekts (Eckpunktkoordinaten, Normalen, Texturkoordinaten -> Uebung 6)
// koennen in separatem Speicher einer Grafikkarte gehalten werden (haengt vom Modell ab)
// koennen ueber API-Aufrufe veraendert werden --> glBufferData, glMapBuffer (GL_STATIC_DRAW, um mitzuteilen, dass sich nichts aendern wird)
// Vertex Array Objects (VAO) (ab 3) kapseln mehrere VBOs eines Objects zwecks Optimierung und einfachrer Benutzung:
// Beim Zeichnen nur 2 Aufrufe: glBindVertexArray und glDrawArrays
//#define UEBUNG5TEDDY  /* Nach der Teddy-�bung fuer Uebung6 wieder ausschalten */
// Alternativ: Teddy wenn die Zeit reicht: google nach teddy chameleon, teapot.obj mit xxx.obj austauschen....
// Modellieren kann also auch einfach sein, Freies Tool Blender (open Source), Professionelle Werkzeuge Maya, 3dsmax, etc. (nicht gegenstand der LV)
#define UEBUNG6 /* Beleuchten, neuer Shader */
// Teddy-Modell hat keine Normalen, passt nicht zu Shadern, wieder zur Teekanne zurueck.
// Shader anschauen, Alter Shader "ColourFragmentShader" setzt Farbe direkt, wird interpoliert ("Gouraud-Shading")
// "TransformVertexShader" gibt Farben weiter, legt layout der Eingabe-Daten fest, verwendet MVP um Eckpunkte zu transvormieren ("Eine Matrix reicht")
// Neue Shader koennen mehr (Immer noch nicht, was die Fixed-Function-Pipeline konnte) 
// Normalen stehen senkrecht auf Oberflaechen, wichtig fuer die Beleuchtung
// Normalen muessen anders behandelt werden als Eckpunkte, deshalb Trennung von MVP in Model View Projection
// -> Shader ver�ndern in Uebung 15
#define UEBUNG7 /* Texturieren */
// Farbtexturen sind digitale Rasterbilder (andere Texturen in Kombination moeglich "Multi-Texturing" nicht Gegenstand der Uebung -> VL Textur)
// Imageloader fuer png und jpg nicht in den Bibliotheken enthalten -> SOIL
// DDS-Format kompatibel zu Texturkompression der Hardware. Wir nehmen aber BMP !
// s. a.:  http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/, Achtung glfwLoadTexture2D gibt' nicht mehr in glfw-version 3
// Granularitaet: Textur pro Objekt. kann also gewechselt werden ! --> Uebung 8
// Texturierte Teekanne ist 2. Teil des Pflichtteils 
///////////////////////////////////////////////////////
#define UEBUNG8 /* Mehrere Objekte */
// Polygone bilden Objekt, Objekte haben eigene Transformationen ("Model"-Matrix) eigene Texturen, etc.
// VAOs abwechselnd binden, MVP-Matrizen abwechselnd setzen, ggf Texturen abwechselnd binden, OpenGL verwendet jeweils positive Zahlen als
// Namen, die man sich generieren lassen muss.
// Model-Matrix fuer jedes Objekt anders, Szenen haben meist hierarchische Struktur, Multiplikationskette von der Wurzel zum Blatt
// Roboter-Beispiel, um Ketten zu lernen
#define UEBUNG9 /* Koordinatenachse */
// Notwendigkeit Koordinatensysteme anzeigen zu lassen... -> drawCS-Funktion mit drei Balkenen
// Wie erhaelt man langen duennen Balken mit bekannten Geometrien ?
// Aufgabe: Balken (leider mit M, V, P, ... als Parameter) Hinweis ginge auch als Singleton...
// Skalierungsparameter ruhig ausprobieren.
// sieht man den Balken im Bauch der Teekanne ?
#define UEBUNG10 /* Koordinatenkreuz */
// Aufgabe: Drei Balken, spaeter entsteht die Notwendigkeit Matrizen zu sichern (evtl. Mechanismus von OpenGL1 erwaehnen)
#define UEBUNG11 /* Ein Robotersegment */
// Teekanne ausblenden, Kugel zeichnen, Transformationsreihenfolge nochmal thematisieren
// Aufgabe: -Funktion mit Parameter!
#define UEBUNG12 /* Drei Robotersegmente uebereinander */
// Aufgabe: statischer Roboter, Unterschied lokales vs. globales Translate
#define UEBUNG13 /* Rotationen in den Gelenken */
// Aufgabe Roboter mit Tastensteuerung, Reihenfolge der Transformationen von oben nach unten und umgekehrt mit Stiften erl�utern
#define UEBUNG14 /* Lampe am Ende */
// Uebung 15 im StandardShading pixelshader
// Hier mal exemplarisch den Pixelshader aendern, um die Beleuchtung ansprechender zu machen
// Erwaehnen, dass man Parameter wie "MVP" kontrollieren koennte.
// Hier beginnt die Wellt der Shader-programmierung, die nicht Gegenstand der Uebung ist.
// Lampe am Ende eines steuerbaren Roboters ist dritter Teil des Pflichtteils
// (5 Punkte: 3 Rotationen, Teekanne, texturiert, Roboter, Licht am Ende) 

// wenn Fehler passiert
void error_callback(int error, const char* description)
{
	fputs(description, stderr); //println
}

#ifdef UEBUNG2
	#ifdef UEBUNG3
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	#else
	float angle = 0;
	#endif
#endif

#ifdef UEBUNG13
	float angle0 = 0.0;
	float angle1 = 0.0;
	float angle2 = 0.0;
	float angle3 = 0.0;
#endif // UEBUNG13


// when we press the button (Escape) -> close
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
#ifdef UEBUNG2
#ifdef UEBUNG3
	case GLFW_KEY_X:
		angle_x += 5;
		break;
	case GLFW_KEY_Y:
		angle_y += 5;
		break;
	case GLFW_KEY_Z:
		angle_z += 5;
		break;

#endif // UEBUNG2
#endif // UEBUNG3

#ifdef UEBUNG13
	case GLFW_KEY_A:
		angle0 += 5;
		break;
	case GLFW_KEY_B:
		angle1 += 5;
		break;
	case GLFW_KEY_C:
		angle2 += 5;
		break;
	case GLFW_KEY_D:
		angle3 += 5;
		break;


#endif // UEBUNG13

	default:
		break;
}
}





// Diese Drei Matrizen global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen. 4x4
glm::mat4 Projection; // Projection matrix - camera (wie weit kann ich gucken, wie rotieren etc.) - Kamera Einstellungen
glm::mat4 View; // camera auch, beschreibt position der Kamera (rotation, sclalier..., position) 
glm::mat4 Model; 
GLuint programID; // program id

void sendMVP()
{
	// Our ModelViewProjection : multiplication of our 3 matrices. Zu Grafikkarte geschikt damit es weis, wo alles ist
	glm::mat4 MVP = Projection * View * Model; 
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform, konstant fuer alle Eckpunkte
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

#ifdef UEBUNG6
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
#endif // UEBUNG6

}

#ifdef UEBUNG5
	#include "objloader.hpp"
#endif

#ifdef UEBUNG7
	#include "texture.hpp";
#endif // UEBUNG7

#ifdef UEBUNG9
void drawCS() {
	glm::mat4  Save = Model;
	Model = glm::scale(Save, glm::vec3(2.0, 0.01, 0.01));
	sendMVP();
	drawWireCube();

	// UEBUNG10
	Model = glm::scale(Save, glm::vec3(0.01, 2.0, 0.01));
	sendMVP();
	drawWireCube();

	Model = glm::scale(Save, glm::vec3(0.01, 0.01, 2.0));
	sendMVP();
	drawWireCube();
}
 #endif // UEBUNG9

#ifdef UEBUNG11
void drawSeg(float height)
{
	glm::mat4 Save = Model;
	Model = glm::translate(Model, glm::vec3(0, height / 2, 0));
	Model = glm::scale(Model, glm::vec3(height / 6, height / 2, height / 6));
	sendMVP(); //zu GrafikKarte
	drawSphere(10, 10);
	Model = Save;
}
#endif // UEBUNG11




int main(void)
{
	// Initialise GLFW. initialize opengl fenster
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE); // exit if fail to initialize
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	glfwSetErrorCallback(error_callback);

	// Open a window and create its OpenGL context
	// glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen
	GLFWwindow* window = glfwCreateWindow(1024, // Breite
										  768,  // Hoehe
										  "CG - Tutorial", // Ueberschrift
										  NULL,  // windowed mode
										  NULL); // shared window - mehr Programmen of Fenster zugreifen. benutzen wir nicht

	if (!window)
	{
		glfwTerminate(); 
		exit(EXIT_FAILURE);
	}

	// Make the window's context current (wird nicht automatisch gemacht)
    glfwMakeContextCurrent(window);

	// Initialize GLEW
	// GLEW erm�glicht Zugriff auf OpenGL-API > 1.1
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Auf Keyboard-Events reagieren
	glfwSetKeyCallback(window, key_callback);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

#ifdef UEBUNG4
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
#endif

#ifdef UEBUNG6
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
#else // 
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
#endif

	// Shader auch benutzen !
	glUseProgram(programID);

#ifdef  UEBUNG5
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	bool res = loadOBJ("teapot.obj", vertices, uvs, normals);

	GLuint VertexArrayIDTeapot;
	glGenVertexArrays(1, &VertexArrayIDTeapot);// & ist Zeiger Addresse
	glBindVertexArray(VertexArrayIDTeapot); // f�r jeder 3d Objekt

	// in buffer �bertragen in puffer f�r GPU
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // binden buffer
	// �bertragen
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW); // buffer data

	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, // index shader
		3, // anzahl werte aus dem vertex array (xyz)
		GL_FLOAT, // typ
		GL_FALSE,//nicht wichtig: normalisierung
		0,
		(void*)0); //undefinierte typ von Pointer. Pointer kann beliebiger typ ahnemen

        #ifdef UEBUNG6
        	GLuint normalBuffer;
        	glGenBuffers(1, &normalBuffer);// buffer
        	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        	glEnableVertexAttribArray(2);
        	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        #endif // UEBUNG6

#ifdef UEBUNG7
			GLuint uvBuffer;
			glGenBuffers(1, &uvBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
			glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			GLuint Texture = loadBMP_custom("mandrill.bmp");
#endif // UEBUNG7


#endif //  UEBUNG5



#ifdef UEBUNG14
			glm::mat4 lightTrf(1.0f);
#endif // UEBUNG14


	// Eventloop. Endloss schleife (weil Jedes Frame weil Fenster offnen ist)
	while (!glfwWindowShouldClose(window))
	{
#ifdef UEBUNG4
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);
#endif

	

		// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		
		// Camera matrix
		View = glm::lookAt(glm::vec3(0,0,-5), // Camera is at (0,0,-5), in World Space
						   glm::vec3(0,0,0),  // and looks at the origin
						   glm::vec3(0,1,0)); // Head is up (set to 0,-1,0 to look upside-down)
		
		// Model matrix : an identity matrix (model will be at the origin)
		Model = glm::mat4(1.0f); // Einheitsmatrix

#ifdef UEBUNG1
	#ifdef UEBUNG2
		#ifdef UEBUNG3
		Model = glm::rotate(Model, angle_x, glm::vec3(1.0, 0.0, 0.0)); // Model, 30%, xyz
		Model = glm::rotate(Model, angle_y, glm::vec3(0.0, 1.0, 0.0)); // Model, 30%, xyz
		Model = glm::rotate(Model, angle_z, glm::vec3(0.0, 0.0, 1.0)); // Model, 30%, xyz
		#else
			Model = glm::rotate(Model, angle, glm::vec3(0.0, 1.0, 0.0)); // Model, 30%, xyz
		#endif
	#else
		Model = glm::rotate(Model, 30.0f, glm::vec3(0.0, 1.0, 0.0)); // Model, 30%, xyz
	#endif
#endif

#ifdef UEBUNG8
		glm::mat4 Save = Model;
		Model = glm::translate(Model, glm::vec3(1.5, 0, 0));
#endif // UEBUNG8


#ifdef UEBUNG5
		Model = glm::scale(Model, glm::vec3(1.0 / 1000, 1.0 / 1000, 1.0 / 1000));
#endif

		sendMVP();

#ifdef UEBUNG6

#ifdef UEBUNG14
		glm::vec4 lightPos = lightTrf * glm::vec4(0, 0, 0, 1);
#else
		glm::vec3 lightPos = glm::vec3(4, 4, -4);
#endif // UEBUNG14

		
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
#endif

#ifdef UEBUNG7
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);
#endif // UEBUNG7

#ifdef UEBUNG5
		glBindVertexArray(VertexArrayIDTeapot);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
#else


#ifdef UEBUNG4
		drawCube();
#else
		drawWireCube();
#endif
#endif

		drawWireCube();	// gezeigt

#ifdef UEBUNG8
		Model = Save;
		Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
#ifdef UEBUNG11

#ifdef UEBUNG13
		Model = glm::rotate(Model, angle0, glm::vec3(0.0, 1.0, 0.0));
		Model = glm::rotate(Model, angle1, glm::vec3(0.0, 0.0, 1.0));
#endif // UEBUNG13
		drawSeg(1.0);
#ifdef UEBUNG12
		Model = glm::translate(Model, glm::vec3(0, 1.0, 0)); // Translate auf die Spitze vom letzten Objekt

#ifdef UEBUNG13
		Model = glm::rotate(Model, angle2, glm::vec3(0.0, 0.0, 1.0));
#endif // UEBUNG13
		drawSeg(0.8);// Zeiche das Objekt
		Model = glm::translate(Model, glm::vec3(0, 0.8, 0)); // Translate auf die Spitze vom letzten Objekt

#ifdef UEBUNG13
		Model = glm::rotate(Model, angle3, glm::vec3(0.0, 0.0, 1.0));
#endif // UEBUNG13
		drawSeg(0.6);// Zeiche das Objekt

#ifdef UEBUNG14
		lightTrf = glm::translate(Model, glm::vec3(0, 0.6, 0));
#endif // UEBUNG14

		Model = Save;
#endif // UEBUNG12

#else
		sendMVP();
		drawSphere(10, 10);

#endif // UEBUNG11



#endif // UEBUNG8

#ifdef UEBUNG9
		drawCS();
#endif // UEBUNG9



		// Swap buffers
		glfwSwapBuffers(window);

		// Poll for and process events 
        glfwPollEvents();
	} 

#ifdef UEBUNG5
	glDeleteBuffers(1, &vertexBuffer);
#endif
#ifdef UEBUNG6
	glDeleteBuffers(1, &normalBuffer);
#endif // UEBUNG6

#ifdef UEBUNG7
	glDeleteBuffers(1, &uvBuffer);
#endif // UEBUNG7


	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

