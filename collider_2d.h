#include <stdlib.h>
#include <vector>

enum ColliderType {
    CircleColider,
    BoxColider
};

class Collider2D
{
    public:
        ColliderType ColliderType;

        void onCollision();
        float getCentralValue(int axis);
        float getExtremeValue(int axis, bool min);
        bool isColliding(Collider2D* otherCollider);

    public:
        static bool isColliding(Collider2D* collider1, Collider2D* collider2);
        static void CheckAllCollisions();
        static void CheckAllCollisions(int begin, int end, int axis, int endCount, float average);
        static void cleanup();
        static int  getNumberOfColliders();
    
    private:
        static std::vector<Collider2D*> allCollider2D;
        static int numberOfColliders;

        
};