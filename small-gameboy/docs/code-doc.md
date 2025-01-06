# code explanation

## structure

### source code

```mermaid
classDiagram

    class Controller {
        -OLED oled
        -PS2Button btn

        +void Serve()
    }

    class OLED {
    }

    class PS2Button {
    }

    class GameBase {
        virtual +void runGame()
        virtual +void initGame()
        virtual +void render()
    }

    class GameFactory {
        +GameBase gameFactory()
    }

    class Menu {
        +void moveSelection()
        +void drawMenu()
    }

    Controller --> Menu
    Controller --> GameFactory
    GameFactory --> GameBase
    Controller --* OLED
    Controller --* PS2Button

    class SnakeGame {
        +void runGame()
        +void initGame()
        +void render()
    }

    class WallBallGame {
        +void runGame()
        +void initGame()
        +void render()
    }

    class AirplaneGame{
        +void runGame()
        +void initGame()
        +void render()
    }

    class AdjustSession {
        +void runGame()
        +void initGame()
        +void render()
    }

    class Phy {
        +bool checkSATCollision()
    }

    GameBase <|-- SnakeGame
    GameBase <|-- WallBallGame
    GameBase <|-- AirplaneGame
    GameBase <|-- AdjustSession

    WallBallGame --> Phy
    AirplaneGame --> Phy
```

### physical engine

```mermaid
classDiagram

    class Point {
    }

    class Line {
    }

    class Vector {
    }

    class Shape {
        <<abstract>>
        getVecProject : (short index) -> Vector
        getVecProject : (Vector *v) -> Vector
    }

    class Square {
        getVecProject : (short index) -> Vector
        getVecProject : (Vector *v) -> Vector
    }

    class Circle {
        getVecProject : (short index) -> Vector
        getVecProject : (Vector *v) -> Vector
    }

    class Triangle {
        getVecProject : (short index) -> Vector
        getVecProject : (Vector *v) -> Vector
    }

    Shape --* Vector
    Vector <|-- Line
    Line --* Point

    Shape <|-- Square
    Shape <|-- Circle
    Shape <|-- Triangle

    class Phy {
        +bool checkSATCollision(Shape,Shape)
    }

    Phy --> Shape

end
```

## SAT Collision Detection Algorithm

The SAT (Separating Axis Theorem) collision detection algorithm is a method used to determine if two convex polygons intersect. It works by checking the perpendicular projections of each polygon onto all possible axes and determining if they overlap.

Here's a high-level overview of the SAT collision detection algorithm:

1. **Find All Axes**: Determine all possible axes for which the polygons can be projected. These axes are typically perpendicular to the edges of the polygons.
2. **Project Polygons onto Axes**: For each axis, project both polygons onto that axis and determine if they overlap.
3. **Check Overlap**: If any pair of projections do not overlap, then the polygons do not intersect. Otherwise, they intersect.
