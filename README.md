# CG-Project-You-have-got-a-friend-in-me
An interactive story and animation project written in C++ using OpenGL functions and libraries. It is a schoolwork project for the 'Computer Graphics' course. As the name suggests, this project draws heavy inspiration from the Disney and Pixar movie Toy Story, a childhood treasure that holds countless cherished memories for us. We aimed to encapsulate the movie's essence by crafting a captivating narrative that emphasizes the enduring bonds of friendship.

To bring our vision to life, we utilized basic shapes such as GL_QUADS, GL_LINES, GL_LINE_LOOP, GL_POLYGON, etc., for character and scene creation. Additionally, we employed functions like drawFilledCircle() and drawHollowCircle()—the former using GL_TRIANGLE_FAN to draw a circle and the latter using GL_LINE_LOOP to outline it. More comprehensive details can be found in our report's PDF file.

However, despite our enthusiasm and efforts, we encountered significant challenges during development. Time constraints posed the primary hurdle, particularly with the intricate designs of characters like Woody and Jessie, demanding substantial drawing and planning efforts. Although we initially intended to include 'Buzz Lightyear,' concerns about time allocation and workload balance led to the decision to omit this character. Furthermore, our plan to incorporate a tic-tac-toe game after the planning scene, where Woody and Jessie determine who climbs out the window first, faced obstacles due to logic errors. Regrettably, due to time limitations and unresolved issues, we had to abandon this idea.

Our motto is, as a wise toy once said, friendships last 'To Infinity and Beyond!'.

Read Before Installing and Running The Program
Microsoft Visual Studio 2022 is what we used to write and run this program but any C++ IDE will work just fine. If you decide to use Visual Studio you need to do the following steps before running the program:

1- Create a new project:

• Choose the 'Empty Project' C++ option and name it as desired. Preferably, choose a name that represents it.

2- Accessing Package Manager:

• In the project window, navigate to 'Tools' >> 'NuGet Package Manager' >> 'Package Manager Settings.'

3- Configuring Package Sources:

• In the appearing window under 'General,' click 'Clear All NuGet Cache(s).'

• Locate 'Package Sources' within the same window.

• Click the '+' button to add a new source:

-Modify the Name to "nuget.org" and the Source to "https://api.nuget.org/v3-index/index.json."

-Remove any other packages to prevent issues.

4- Finalizing Package Setup:

• Click 'OK,' return to the project window, and revisit 'Tools' >> 'NuGet Package Manager' >> 'Package Manager Console.'

• Wait for the Console to appear and, once "PM>" shows up, input: "Install-Package nupengl.core."

• Wait until it's successfully installed.

Regarding the code
Before running the program, follow these steps:

1- Locating the Texture Image:

• Locate and ensure you have the required texture image (woodenTexture.bmp) downloaded onto your device.

2- Handling the Texture Image in the Code:

• Copy the image's path and ensure it's correctly placed within the code (Line 48) to prevent errors.

By completing these steps, you'll be ready to run our code to give it a try!

User Manual
First: Mouse Clicking Actions:

In the starting scene,

• Click the 'Click to start' button using the left mouse.

In the remaining scenes (except the ending),

• Click the 'Next' button using the left mouse.

In the planning scene,

• Use the red device buttons:

• Right device button:

-Right mouse click to reveal the plan.

-Left mouse click for a secret message from Lotso.

• Left device button:

-Left or right mouse click to clear the device screen.

Second: Keyboard Pressing Actions:

In the window and neighbor's house scenes (scenes 4 and 5),

• Jessie's Movement:

-Use the arrow keys to move Jessie.

• Woody's Movement:

-Use {d, f, r, c} keys to move Woody left, right, up, and down, respectively.

Regarding The Presentation and Demo
Presentation Details:

-Our presentation is cinema/movie-themed, noticeable as the lights dim at the beginning and the credits roll at the end, adding to the overall beauty of the project.

-We carefully crafted the scenario of the presentation, serving as the basis for the actual coded story, ensuring a cohesive experience.

-Download the (CG-PP(you've got a friend in me).pptx) file to witness the transitions, plot, and our creative approach in showcasing the project to the audience.

Have fun and enjoy the experience!!

Demo Inclusion:

-The demo is seamlessly integrated into the presentation as well as available separately in the project files for your convenience.

-For the voiceover, we used an AI voice-generator tool referenced in the end.

Credits
The team members involved in creating this work are:

• Aseel Keleeb - GitHub Profile: https://github.com/AseelKeleeb

• Budur Alghamdi - GitHub Profile: https://github.com/budu6655

• Noran Almughamisi - GitHub Profile: https://github.com/NoranAlmughamisi

• Teif Alhrbi

• Nrdeen Sahrah

Special thanks and credit are extended to our professor, Omniah Nagoor, whose invaluable assistance greatly contributed to the development of this project.

Other Resources and References
• Drawing of a circle: https://gist.github.com/linusthe3rd/803118

• DrawString method: https://www.openglprojects.in/2012/04/tutorial-how-to-display-strings-in.html#gsc.tab=0

• Font Type: https://www.opengl.org/resources/libraries/glut/spec3/node76.html

• Texture code: https://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture

• Texture image: https://www.pinterest.cl/pin/349591989837410639/

• Website to convert image to BMP: https://convertio.co/ar/jpg-bmp/

• To have a better understanding of the glRasterPos2f(x, y): https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glRasterPos.xml

• Voiceover of the demo: https://www.narakeet.com/app/text-to-audio/?projectId=0889efa6-9cb3-429b-9669-526f8997e862

Copyright © 2023 Omniah Nagoor, Aseel Keleeb, Budur Alghamdi, Noran Almughamisi, Teif Alhrbi, and Nrdeen Sahrah. All rights reserved.
