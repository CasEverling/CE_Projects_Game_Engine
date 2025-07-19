#include <stdlib.h>
#include <vector>
#include "vector2D.h"

enum ColliderType {
    CircleColider,
    BoxColider
};

class Collider2D
{
    private:
        Collider2D();

    public:        
        std::shared_ptr<Collider2D> create();

        ColliderType ColliderType;

        void onCollision();
        float getCentralValue(int axis);
        Vector2D getExtremeValue(int axis);
        bool isColliding(Collider2D* otherCollider);




    public:
        static bool isColliding(std::shared_ptr<Collider2D> collider1, std::shared_ptr<Collider2D> collider2);

        static void CheckAllCollisions();
        static void CheckAllCollisionsMultiThread();
        

        static void cleanup();
        static int  getNumberOfColliders();
    
    private:
        static void CheckAllCollisions(bool isMultiThread);

        static void CheckAllCollisions(std::vector<std::size_t>& indexes, int axis, int begin, int size, int endCount, float average, int max_size);
        static void CheckAllCollisionsMultiThread(const std::vector<std::size_t>& indexes, int axis, int size, int endCount, float average, int max_size);
        static void BruteForceCheck(const std::vector<std::size_t>& indexes, int size);
        static void BruteForceCheck(std::vector<std::size_t>& indexes, int begin, int size);

        static std::vector<std::shared_ptr<Collider2D>> allCollider2D;
        static int numberOfColliders;
};