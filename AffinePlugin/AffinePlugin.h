#include "editor_plugin_interface.h"
#include <QObject>
#include "opencv2/opencv.hpp"

class AffinePlugin: public QObject, public EditorPluginInterface{
    Q_OBJECT;
    Q_PLUGIN_METADATA(IID EDIT_PLUGIN_INTERFACE_IID);
    Q_INTERFACES(EditorPluginInterface);
public:
    QString name();
    void edit(const cv::Mat &input, cv::Mat &output);
};
