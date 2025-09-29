******************************************************************* README *****************************************************************************

Usé la librería externa "QCustomPlot" para hacer todo esto. La librería default de gráficos de QT "QtCharts" no me funca y me pudrí de intentar hacerla andar.
Para que no te tire 10 mil millones de errores, bajá los dos archivos "qcustomplot.cpp" y "qcustomplot.h" de https://www.qcustomplot.com/ e incluilos en la carpeta
del proyecto junto con sus respectivos includes en el < .pro >. De todas formas, eso ya está hecho je. Aviso por las dudas(?


Bueno a ver, veamos que hice 👀
El flujo de funcionamiento de esta app QT básicamente es el siguiente
    - Cada X segundos (configurables) un timer ejecuta una función que:
        - Consulta la base de datos
        - Actualiza el gráfico con los datos sensados de presión arterial más recientes (frecuencia cardíaca cof cof)
    - El usuario puede interactuar, esto es:
        - Desplazarse por el gráfico para ver datos del pasado en el mismo día. También se incluye una funcionalidad para hacer zoom
        - Seleccionar fechas pasadas en el calendario para consultar los datos sensados en esas fechas
        - Botón de "Muestreo en Tiempo Real" para ver como se grafican los datos a medida que el QT los levanta de la base de datos
        - Hay un segundo timer que detecta inactividad en caso de que el usuario previamente haya interactuado, y tras unos segundos, automáticamente se devuelve la vista al
          modo de Muestreo en Tiempo Real
    - Como todavía no tengo sensor real funcionando, se simularon los datos:
        - Se generan datos truchos cada 1 segundo mediante el script "client.js" -> se mandan por http al "server.js" -> los almacena en la base de datos "file_past.db"
        - Qt los levanta y los grafica
Hasta acá no hay nada nuevo, basicamente hice lo mismo que hiciste vos el martes pasado en clase, pero con la librería QCustomPlot (no recuerdo si habias podido llegar a hacer los plots)

Habría que discutir posibles mejoras y funcionalidades a agregar
Lo que me falta implementar en el código:
    - Poder filtrar muestras por rango de tiempo desde la ventana de la app. No se, que se puedan visualizar los datos de los útlimos 5 minutos, 30 minutos o 24 horas, por 	  
      ejemplo. Incluso también poder seleccionar un período de días
    - Poder exportar los datos sensados a un archivo, supongo que tipo excel
Estas dos útlimas funcionalidades vi que las tenías en el .ui de la app qt que subiste al git
Otra idea que espero que no sea dificil de implementar, es:
    - Mostrar estadísticas como por ejemplo valores máximos, mínimos, promedio del día, etc etc

Te dejo un drive para que veas unas fotitos de como se ve el gráfico https://drive.google.com/file/d/1oIaZklMZ8puriAsaVuAebh9BmfiS9rK9/view?usp=sharing

PD: traté de comentar el código de QT lo más que pude, espero que sirva 

********************************************************************************************************************************************************
