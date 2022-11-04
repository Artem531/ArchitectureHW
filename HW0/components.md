# Компонентная архитектура
<!-- Состав и взаимосвязи компонентов системы между собой и внешними системами с указанием протоколов, ключевые технологии, используемые для реализации компонентов.
Диаграмма контейнеров C4 и текстовое описание. 
-->
## Компонентная диаграмма

```plantuml
@startuml
!include https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Container.puml

AddElementTag("microService", $shape=EightSidedShape(), $bgColor="CornflowerBlue", $fontColor="white", $legendText="microservice")
AddElementTag("storage", $shape=RoundedBoxShape(), $bgColor="lightSkyBlue", $fontColor="white")

Person(admin, "Администратор")
Person(seller, "Продавец")
Person(user, "Пользователь")

System_Ext(order_checker, "Сервис проверки нового заказа")
System_Ext(new_service_checker, "Сервис проверки новой услуги")
System_Ext(new_user_checker, "Сервис проверки нового пользователя")

System_Boundary(shop, "Магазин с услугами") {
   Container(app, "Клиентское веб-приложение", "html, python", "Сайт магазина с услугами")
   Container(new_service_service, "Сервис создания услуг", "C++", "Сервис управления информацией об услуге", $tags = "microService") 
   Container(client_service, "Сервис авторизации", "C++", "Сервис управления пользователями", $tags = "microService")    
   Container(new_order_service, "Сервис создания заказа", "C++", "Сервис управления информацией о заказе", $tags = "microService")       
   ContainerDb(db, "Данные сайта", "MySQL", "Хранение данных сайта", $tags = "storage")
}

Rel(admin, app, "Добавление/просмотр информации о пользователях")
Rel(seller, app,"Создание новой услуги, получение заказа для пользователя")
Rel(user, app,"Получение списка услуг, добавление услуги в заказ")

Rel(app,new_service_service,"Работа с услугами","http://localhost/service ")
Rel_L(new_service_service,new_service_checker,"Запрос информации о новой услуге")
Rel(new_service_service,db,,"INTSERT/SELECT/UPDATE","SQL")

Rel(app,new_order_service,"Работа с заказами","http://localhost/order ")
Rel_R(new_order_service,order_checker,"Запрос нового заказа")
Rel(new_order_service,db,"INTSERT/SELECT/UPDATE","SQL")

Rel(app,client_service,"Работа с новым пользователем","http://localhost/user ")
Rel_R(client_service,new_user_checker,"Запрос нового пользователя")
Rel(client_service,db,"INTSERT/SELECT/UPDATE","SQL")


@enduml
```
## Список компонентов
### Сервис авторизации

**API**:
-	Создание нового пользователя
      - входыне параметры: login, Имя, Фамилия, email, обращение (гн/гжа)
      - выходные параметры, отсутствуют
-	Поиск пользователя по логину
     - входные параметры:  login
     - выходные параметры: Имя, Фамилия, email, обращение (гн/гжа)
-	Поиск пользователя по маске имя и фамилии
     - входные параметры: маска фамилии, маска имени
     - выходные параметры: login, Имя, Фамилия, email, обращение (гн/гжа)

### Сервис создания услуг
**API**:
- Сохранение информации о новой услуге
  - Входные параметры: Описание услуги и ее цена
  - Выходыне параметры: отсутствуют
- Получение информации о услугах
  - Входные параметры: диапазон цен
  - Выходные параметры: массив пар цена - услуга


### Сервис создания заказа
**API**:
- Сохранение информации о измерениях в заказе
  - Входные параметры: название услуги
  - Выходные параметры: отсутствуют
- Получение информации о заказах
  - Входные параметры: диапазон дат
  - Выходные параметры: массив пар время - номер заказа 
