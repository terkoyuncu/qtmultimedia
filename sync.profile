%modules = ( # path to module name map
    "QtMultimedia" => "$basedir/src/multimedia",
    "QtMultimediaWidgets" => "$basedir/src/multimediawidgets",
    "QtMultimediaQuick" => "$basedir/src/multimediaquick",
    "QtQuick3DAudio" => "$basedir/src/multimediaquick3d",
);

%moduleheaders = ( # restrict the module headers to those found in relative path
);

%classnames = (
    "qaudio.h" => "QAudio",
    "qmediametadata.h" => "QMediaMetaData",
    "qmultimedia.h" => "QMultimedia"
);
%deprecatedheaders = (
    "QtMultimedia" =>  {
        "qtmultimediadefs.h" => "QtMultimedia/qtmultimediaglobal.h"
    },
    "QtMultimediaWidgets" =>  {
        "qtmultimediawidgetdefs.h" => "QtMultimediaWidgets/qtmultimediawidgetsglobal.h"
    }
);
