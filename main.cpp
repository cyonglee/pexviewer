#include "mainwindow.h"

#include <QApplication>
#include <QVulkanInstance>
#include <fstream>
#include <string>
#include <iostream>
#include "formtop.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QVulkanInstance inst;

#ifndef Q_OS_ANDROID
    inst.setLayers(QByteArrayList() << "VK_LAYER_LUNARG_standard_validation");
#else
    inst.setLayers(QByteArrayList()
                   << "VK_LAYER_GOOGLE_threading"
                   << "VK_LAYER_LUNARG_parameter_validation"
                   << "VK_LAYER_LUNARG_object_tracker"
                   << "VK_LAYER_LUNARG_core_validation"
                   << "VK_LAYER_LUNARG_image"
                   << "VK_LAYER_LUNARG_swapchain"
                   << "VK_LAYER_GOOGLE_unique_objects");
#endif

    if (!inst.create())
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

    VulkanWindow *vulkanWindow = new VulkanWindow;
    vulkanWindow->setVulkanInstance(&inst);

    MainWindow mainWindow(vulkanWindow);
    QObject::connect(vulkanWindow, &VulkanWindow::signalInfoText, &mainWindow, &MainWindow::slotInfoText);

    QRect size = mainWindow.geometry();
    mainWindow.shareGeo(size);
    qDebug() << "Main geo : " << mainWindow.geometry();

    mainWindow.show();





    return app.exec();
}
