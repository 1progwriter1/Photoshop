#include "api_impl/bar/bar.hpp"


extern "C"
{


bool onLoadPlugin();
void onUnloadPlugin();


}


class OptionsBar : public AOptionsBar
{
public:
    OptionsBar(wid_t init_id, std::unique_ptr<sfm::IRectangleShape> &main_shape);

    sfm::vec2i calculateNextPos(sfm::vec2i init_pos);
};
