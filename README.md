# Naprendszer Szimuláció (OpenGL C++)

Egy 3D-s, interaktív naprendszer szimuláció, amely C++ és modern OpenGL (Core Profile) használatával készült. A projekt egy szabadon mozgatható belső nézetes kamerarendszert, textúrázott bolygómodelleket, dinamikus világítást és alapvető ütközésvizsgálatot tartalmaz.

## 🚀 Funkciók

*   **3D Renderelés:** Kockákból felépített, textúrázott égitestek és egy galaxis háttér (`stb_image` könyvtár használatával).
*   **Dinamikus Bolygómozgás:** A bolygók egyedi sebességgel, pályasugárral és forgási sebességgel keringenek a középpont (Nap) körül a `glfwGetTime()` alapján.
*   **FPS Kamerarendszer:** Szabad repülés a térben az egér és a billentyűzet segítségével (Pitch/Yaw számítások).
*   **Dinamikus Világítás (Phong Lighting Model):** Távolságtól függő (attenuation) környezeti (ambient), szórt (diffuse) és tükröződő (specular) fények.
*   **Ütközésvizsgálat (Collision Detection):** A program méri a kamera és a bolygók távolságát. Túl közeli érintkezés esetén a játék véget ér (Game Over képernyő).

## 🛠️ Használt Technológiák és Könyvtárak

*   **C++14 / C++17**
*   **OpenGL 4.0+** (Core Profile)
*   **GLFW:** Ablakkezelés és felhasználói bemenetek (billentyűzet, egér) feldolgozása.
*   **GLAD:** OpenGL függvények betöltése.
*   **GLM (OpenGL Mathematics):** Mátrix- és vektorműveletek (vektorok, forgatások, transzformációk).
*   **stb_image:** Képfájlok (JPG, PNG) betöltése a textúrákhoz.

## 🎮 Irányítás

| Gomb / Eszköz | Akció |
| :--- | :--- |
| **W, A, S, D** | Mozgás előre, balra, hátra, jobbra |
| **Egér mozgatása** | Kamera forgatása (Nézelődés) |
| **Egér görgő** | Látószög (FOV / Zoom) állítása |
| **Esc** | Kilépés a programból |
| **Enter** | Újraindítás (Game Over képernyőnél) |

## ⚙️ Futtatás és Telepítés

A projekt Visual Studio környezetben készült. A sikeres fordításhoz és futtatáshoz a következő beállítások szükségesek:

1. Klónozd a tárolót:
   ```bash
   git clone [https://github.com/felhasznaloneved/repo-neve.git](https://github.com/felhasznaloneved/repo-neve.git)