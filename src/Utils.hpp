#pragma once

using namespace geode::prelude;

constexpr const char* infoStr = R"(# Icon Workbench (ENG)
Welcome to the ***Icon Workbench Menu***!!
In here, you will find all the mod's tools to assist you with your Icon Creating process! You've probably already read the mod's info page, so i'll just give you some extra guidance on what everything here does!

In the Main Popup, you will find various things, such as the **Offset Input Fields**, which is where you can edit your Icon Sprite Offsets to your liking! 
To the right side of the text inputs you will see the icon you're currently editing. This is the **Live Preview**, where your offset changes will be applied live.
This **Live Preview** will also be where you'll be able to preview the Robot/Spider animations, ball rolling, and Cube position!

Right under the main popup you will find 2 buttons, labeled "Update" and "Apply":

- The "Update" button will apply any changes you've made to the Frame Offsets in the Popup's input fields. (Or, in the mod's settings, u can find a setting to enable automatically applying changes any time the Input fields changes! This is off by default due to stability concerns).
- The "Apply" button will automatically edit your Icon's plist file to apply your changed offsets!

At the right side of the mod's popup you will see a little *"Side-menu"*, this is where you can choose which Icon Sprite to edit! When you click on one, it'll be selected and show a small flashing animation to show it's the currently selected Sprite, and of course, once an icon is selected, the Text Inputs will edit that Sprite's offsets.

Under the Live Preview you will find 2 buttons, these essentially act as Togglers for the **Live Preview's Glow** and the **Hitbox Preview**.

At the left side of the menu, outside the Main Popup you will find 3 Buttons, these will allow you to change the **Live Preview Colors** directly from the Mod's popup!

At the top right corner of the main Popup there's a small menu (which you already saw because u clicked this button) where you will find 3 buttons:
- The first one shows this info popup (hi :3)
- The second one opens your chosen Renders Folder.
- The third one is the **Render Icon Button**!

The **Render Icon** button will immediately render your Live Preview right as it is in its current state to a PNG image! It'll be saved to your chosen Renders folder, which you can customize via the mod's settings menu. By default, they'll be saved to a "Renders" folder inside the mod's config folder.

# Icon Workbench (ESP) [Traducido con DeepL]
¡Bienvenido al menu del ***Icon Workbench***!
Aqui encontraras todas las herramientas del mod que te ayudaran en el proceso de creacion de iconos. Probablemente ya hayas leido la pagina de informacion del mod (espero), asi que solo te dare algunas indicaciones adicionales sobre lo que hace cada cosa aqui.

En la ventana principal encontraras varias cosas, como los **Campos de entrada de los Offsets**, donde puedes editar los offsets de los sprites de los iconos a tu gusto. 
A la derecha de los campos de entrada de texto veras el icono que estas editando actualmente. Se trata de la **vista previa en vivo**, donde se aplicaran en tiempo real los cambios de desplazamiento.
Esta **vista previa en vivo** tambien te permitira previsualizar las animaciones del robot/arana, el movimiento de la bola y la posicion del cubo.

Justo debajo de la ventana principal encontraras dos botones, etiquetados como "Actualizar" y "Aplicar":

- El boton "Actualizar" aplicara cualquier cambio que hayas realizado en los offsets de cada frame en los campos de entrada de la ventana. (O, en la configuracion del mod, puedes encontrar una opcion para habilitar la aplicacion automatica de los cambios cada vez que se modifiquen los campos de entrada. Esta opcion esta desactivada de forma predeterminada por motivos de estabilidad).
- El boton "Aplicar" editara automaticamente el archivo plist de tu icono para aplicar los desplazamientos modificados.

En la parte derecha de la ventana del mod veras un pequeno *"menu lateral"*, Aqui es donde puedes elegir que sprite de tu icono editar! Cuando hagas clic en uno, se seleccionara y mostrara una pequena animacion parpadeante para indicar que es el sprite seleccionado actualmente y, por supuesto, una vez seleccionado un icono, las entradas de texto editaran los offsets de ese sprite.

Debajo de la vista previa en vivo encontraras dos botones que actuan esencialmente como interruptores para activar/desactivar la **preview del glow** y la **preview de la hitbox**.

En la parte izquierda del menu, fuera de la ventana principal, encontraras tres botones que te permitiran cambiar los **colores de la vista previa en vivo** directamente desde la ventana emergente del mod.

En la esquina superior derecha de la ventana principal hay un pequeno menu (que ya has visto porque has hecho clic en este boton) donde encontraras tres botones:
- El primero muestra esta ventana de informacion (holi :3).
- El segundo abre la carpeta de renders que hayas elegido.
- ¡El tercero es el boton de **Renderizar Icono**! (La camarita)

El boton **Renderizar Icono** renderizara inmediatamente tu vista previa en vivo tal y como esta en su estado actual a una imagen PNG. Se guardara en la carpeta de renderizados que hayas elegido, que puedes personalizar a traves del menu de configuracion del mod. Por defecto, se guardaran en una carpeta «Renders» dentro de la carpeta de configuracion del mod.)";

constexpr const char* whyStr = R"(## Why aren't vanilla icons supported? (ENG)

The **More Icons** mod provides simple and quick ways to know various things this mod heavily utilizes:

- Where is the .plist file for an Icon located
- A full list of frame names for an Icon
- The Icon's name
- When necessary, the Texture Pack's ID/Name

Fetching these things without More Icons for Vanilla Icons would get very annoying VERY quickly. Therefore, the mod relies on More Icons to function properly.

The mod's main target audience is Icon **Creators** anyway, so, most creators are probably using More Icons to load their icons without touching their vanilla icon list (Or should be, at least. Trust me, it's a whoooole 'nother world.)

In any case, if you're making a Vanilla Icon Pack, i'd recommend enabling More Icons' "Load from Traditional Icon Packs" setting, at least temporarily. This will load icons from Vanilla icon packs as if they were More Icons added Icons, and therefore, you SHOULD be able to edit them via Icon Construct. From my testing this DOES work, so it should for you too!

## ¿Por que no se admiten los iconos vanilla? (ESP) [Traducido con DeepL]

El mod **More Icons** proporciona formas sencillas y rapidas de conocer diversos aspectos de los iconos que este mod utiliza en gran medida:

- Donde se encuentra el archivo .plist de un icono
- Una lista completa de los nombres de cada frame/parte de un icono
- El nombre del icono
- Cuando sea necesario, el ID/nombre del Texture Pack

Obtener esta informacion sin More Icons para los iconos predeterminados resultaria muy molesto MUY rapidamente. Por lo tanto, el mod depende de More Icons para funcionar correctamente.

De todos modos, el publico principal al que se dirige el mod son los **creadores** de iconos, por lo que es probable que la mayoria de ellos utilicen More Icons para cargar sus iconos sin tocar su lista de iconos predeterminados (o al menos **deberian** de hacerlo. Creeme, es otro cantar).

En cualquier caso, si estas creando un paquete de iconos Vanilla, te recomiendo que actives la configuracion "Cargar desde paquetes de iconos tradicionales" de More Icons, al menos temporalmente. Esto cargara los iconos de los paquetes de iconos Vanilla como si fueran iconos anadidos por More Icons y, por lo tanto, DEBERiAS poder editarlos a traves de Icon Construct. Segun mis pruebas, esto FUNCIONA, ¡asi que tambien deberia funcionar para ti!)";

constexpr int FALLBACK_TAG = 105871529;