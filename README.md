# QDNav
## Indoor-навигатор

Недостатком популярных навигаторов, предназначенных для нахождения маршрутов внутри помещений,
является избыточность информации. Предлагаемый навигатор позволяет вводить схему здания
в XML-файл, а маршрут выводится в текстовом виде. Также изображается упрощенная схема здания.

XML-файл содержит описание пунктов и путей перемещения между ними.
Путь всегда идет по прямой. Вложенные теги `<Path>` означают, что внутренний путь ориентирован
относительно внешнего в определенном направлении.

Разные этажи описываются отдельно друг от друга, в тегах `<Floor>`, но могут содержать идентификаторы
лестниц и лифтов. На разных этажах идентификатор лестницы или лифта один и тот же.

Ориентация карты на каждом этаже задается атрибутом dir у внешнего тега `<Path>`.

[Навигатор на Repl.it](https://replit.com/@grenkin/qdnav?v=1)
