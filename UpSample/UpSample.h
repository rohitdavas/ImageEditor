#include "editor_plugin_interface.h"
#include <opencv2/dnn_superres.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <QObject>
#include "opencv2/opencv.hpp"

class UpSamplePlugin: public QObject, public EditorPluginInterface{
    Q_OBJECT;
    Q_PLUGIN_METADATA(IID EDIT_PLUGIN_INTERFACE_IID);
    Q_INTERFACES(EditorPluginInterface);

public:
    QString name();
    void edit(const cv::Mat &input, cv::Mat &output);
};
