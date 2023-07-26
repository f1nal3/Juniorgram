![](https://trello-attachments.s3.amazonaws.com/5f99a9ec82d4661c1df02e5e/600x499/ff8b9282467aaa8afcf0d3a529d2316a/DB.png)

![](https://trello-attachments.s3.amazonaws.com/5f973f301211050c1336cefc/813x567/22531f32ad348b1bc3d0d46aae4c4406/image.png)  

![](https://trello-attachments.s3.amazonaws.com/5f973f301211050c1336cefc/989x80/6980af534204c41644c1c0076cf839cf/image.png)

Одна из идей, как может выглядеть API к БД на сервере:

БД содержит в себе информацию о зарегистрированных пользователях, конкретно login/email, password hash, token, refresh token, token_receipt_time.

На данный момент это будет сырая версия API, при помощи которого пользователь сможешь зарегистрироваться. авторизоваться в нашем приложении.
Далее БД можно будет расширить при желании как угодно.

API будет, примерно, включать следующие функции :

Response::ResponseCodes authorizeUser(const std::string &LoginOrEmail,const std::string &PasswordHash, std::string& Token, std::string& RefreshToken)

LoginOrEmail - параметр, который содержит в себе либо login, либо email пользователя.

PasswordHash - хэш-строка, полученная хэширования пароля. Функию хэширования можно брать любую(SHA1/SHA256/RIPEMD160 и т.д.)

Token - хэш-строка, предназначенная для удостоверения личности. Она выдаётся пользователю после успешной авторизации в приложении.
(https://ru.wikipedia.org/wiki/Токен_(авторизации)#:~:text=Часто используется для несанкционированного доступа,так и вместе с ним.)

RefreshToken - хэш-строка, предназначенная для обновления токена пользователя, если время действия токена истекло.

Возвращаемое значение функции один из кодов ResponseCodes(enum).
К примеру, при успешной авторизации возвращает Response::ResponseCodes::SUCCESS. В ином случае Response::ResponseCodes::INCORRECT_AUTH_DATA.

Response::ResponseCodes registerUser(const std::string &Email, const std::string &Login, const std::string &Password, std::string& Token, std::string& refreshToken)

Email - параметр, содержащий в себе email пользователя.
Login - логин пользователя.
Password - пароль пользователя.
Token - см. выше.
RefreshToken - см. выше.

При успешной регистрации пользователя ему отдаются его Token и RefreshToken, которые присваиваются в передаваемые переменные Token и RefreshToken.

Возвращаемое значение функции один из кодов ResponseCodes(enum).
Регистрация успешна - Response::ResponseCodes::SUCCESS
Пользователь уже существует - Response::ResponseCodes::USER_ALREADY_EXIST

Response::ResponseCodes validateToken(std::string authToken);

Функция, проверяющая не истёк ли Token, содержащийся в параметре authToken.
Token имеет определённое время, в течении которого он валиден и мы знаем, что именно пользователь, который является владельцем токена может делать запросы на сервер, а не кто-то другой.

Возвращаемое значение функции один из кодов ResponseCodes(enum).
В случае, если строка с токеном пустая Response::ResponseCodes::INVALID_TOKEN
Если токен истёк - Response::ResponseCodes::SESSION_EXPIRED;
Если токен валидный, т.е. время его жизни не истекло - return Response::ResponseCodes::SUCCESS;
В любом другом случае - Response::ResponseCodes::INVALID_TOKEN;

Response::ResponseCodes logout(std::string Token)

Метод, суть которого в удалении у конкретного пользователя в базе поля Token.

Возвращаемое значение - Response::ResponseCodes::SUCCESS.

int refreshToken(std::string& Token, std::string& RefreshToken)

Функция, которая обновляет Token, по RefreshToken.
Для чего эта функция: пользователь решил выйти из приложения, вызывается функция logout - и его токен перестаёт быть валидным и стирается в базе.
Как только пользователь снова залогинился ему нужно получить Token заново. Берётся его Refresh_token(который хранится у него локально. В базе отдельной, или ещё каким образом) и делается автоматически запрос на обновление его токена на сервер. Сервер делает сравнение в базе по пришедшему Refresh_token и содержащемуся в базе, и если они совпали, то выдаёт новый токен пользователю. Следовательно, пользователь прошёл этап авторизации.

Параметрами передаются хэш-строки.

Response::ResponseCodes refresh(const std::string RefreshTokenDB, std::string newToken, std::string newRefreshToken)

Функция для инициализации/обновления полей RefreshToken, Token, TokenReceiptTime(поле - время выдачи токена. Для того, чтобы можно было отследить, жив ли токен) . Инициализирует в базе поля Token и RefreshToken, TokenReceiptTime конкретного пользователя.