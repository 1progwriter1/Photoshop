#include "../headers/api_impl/bar.hpp"


extern "C"
{


bool loadPlugin();
void unloadPlugin();


}


class OptionsBar : public AOptionsBar
{
public:
    OptionsBar(wid_t init_id, std::unique_ptr<sfm::RectangleShape> &main_shape);

    sfm::vec2i calculateNextPos(sfm::vec2i init_pos);
};
