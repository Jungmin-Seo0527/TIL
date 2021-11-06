## 1. 타임리프 - 기본 기능

### 1-1. 프로젝트 생성

### 1-2. 타임리프 소개

#### 타임리프 특징

* 서버 사이드 HTML 렌더링(SSR)
* 네츄럴 템플릿
* 스프링 통합 지원

##### 렌더링

* 서버로부터 HTML 파일을 받아 브라우저에 뿌려주는 과정
* 렌더링 순서
    * HTML을 파싱하여 DOM 트리를 만든다.
    * CSS를 파싱하여 CSSOM 트리를 만든다.
    * DOM과 CSSOM을 결합하여 렌더링 트리를 만든다.
    * 렌더링 트리에서 각 노드의 크기와 위치를 계산한다.
    * 개별 노드를 화면에 그린다.

> 더이상의 설명은 프론트엔드를 공부할 필요가 있어보인다.

[참고 블로그: 렌더링이란](https://velog.io/@ru_bryunak/%EB%A0%8C%EB%8D%94%EB%A7%81%EC%9D%B4%EB%9E%80)

##### 네츄럴 템플릿

* 타임리프는 순수 HTML을 최대한 유지
    * 웹 브라우저에서 파일을 직접 열어 내용확인 가능
    * 서버를 통해 뷰 템플릿을 거치면 동적으로 변경되 결과 확인 가능
* JSP와의 차이점
    * JSP: JSP 소스코드와 HTML이 섞여 있어서 웹 브라우저에서는 정상적인 HTML결과 확인이 어렵다.(서버를 통해 JSP를 렌더링 하는 과정이 필요)
    * 타임리프: 작성된 파일을 그대로 웹 브라우저에서 열어도 정상적인 HTML 결과 확인 가능
    * **순수 HTML을 그대로 유지하면서 뷰 템플릿도 사용할 수 있는 타임리프의 특징을 네츄럴 템플릿이라 한다.**

### 1-3. 텍스트 - text, utext

#### 가장 기본적인 텍스트 출력 방식

* `th:text`사용
    * `<span th:text="${data}">`
    * HTML 테그의 속성에 기능을 정의해서 동작
* 직접 데이터 출력
    * `[[${data}]]`

##### BasicController.java

* `inflearn/SpringMVC2/thymeleaf-basic/thymeleaf-basic/src/main/java/hello/thymeleafbasic/basic/BasicController.java`

```java
package hello.thymeleafbasic.basic;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
@RequestMapping
public class BasicController {

    @GetMapping("basic/text-basic")
    public String textBasic(Model model) {
        model.addAttribute("data", "Hello <b>Spring!</b>");
        return "basic/text-basic";
    }
}
```

##### text-basic.html

* inflearn/SpringMVC2/thymeleaf-basic/thymeleaf-basic/src/main/resources/templates/basic/text-basic.html

```html
<!DOCTYPE html>
<html xmlns:th="http://www.thymeleaf.org>">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<h1>컨텐츠에 데이터 출력하기</h1>
<ul>
    <li>th:text 사용 <span th:text="${data}"></span></li>
    <li>컨텐츠 안에서 직접 출력하기 = [[${data}]]</li>
</ul>
</body>
</html>
```

#### Escape

* html에서 `<`, `>`와 같은 특수 문자를 출력하기 위한 방법
* 웹 브라우저: `Hello <b>Spring!</b>`(`Spring!`문자열이 굵게 나오기를 의도)
* 소스코드: `Hello %lt;b&gt;Spring!&lt;/b&gt;`(위의 문자열이 변환)
    * `<` -> `&lt`

##### HTML 엔티티

* HTML 엔티티
    * 웹 브라우저가 `<`를 태그가 아닌 문자로 표현하는 방법
    * Escape: 특수문자 -> HTML 엔티티
    * `th:text`, `[[...]]`는 **기본적으로 escape을 제공한다.**
        * `<` -> `&lt;`
        * `>` -> `&gt;`
        * 등등...

#### Unescape

* excape기능을 사용하지 않는 방법
    * `th:text` -> `th:utext`
    * `[[...]]` -> `[(...)]`

##### BasicController.java (추가)

```java
package hello.thymeleafbasic.basic;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
@RequestMapping
public class BasicController {

    // ...

    @GetMapping("basic/text-unescaped")
    public String textUnescaped(Model model) {
        model.addAttribute("data", "Hello <b>Spring!</b>");
        return "basic/text-unescaped";
    }
}
```

##### text-unescape.html

* `inflearn/SpringMVC2/thymeleaf-basic/thymeleaf-basic/src/main/resources/templates/basic/text-unescaped.html`

```html
<!DOCTYPE html>
<html xmlns:th="http://www.thymeleaf.org>">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<h1>text vs utext</h1>
<ul>
    <li>th:text = <span th:text="${data}"></span></li>
    <li>th:utext = <span th:utext="${data}"></span></li>
</ul>

<h1><span th:inline="none">[[...]] vs [(...)]</span></h1>
<ul>
    <li><span th:inline="none">[[...]] = </span>[[${data}]]</li>
    <li><span th:inline="none">[(...)] = </span>[(${data})]</li>
</ul>

</body>
</html>
```

* 웹 브라우저: Hello **Spring!**
* 소스 보기: `Hello <b>Spring!</b>`
* 기본적으로 escape을 사용하고 꼭 필요할 때만 unescape을 사용

### 1-4. 변수 - SpringEL

#### 변수 표현식: `${...}`

##### BasicController.java (추가)

```java
package hello.thymeleafbasic.basic;

import lombok.AllArgsConstructor;
import lombok.Data;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Controller
@RequestMapping
public class BasicController {

    // ...

    @GetMapping("basic/variable")
    public String variable(Model model) {
        User userA = new User("userA", 10);
        User userB = new User("userB", 20);

        List<User> list = new ArrayList<>();
        list.add(userA);
        list.add(userB);

        Map<String, User> map = new HashMap<>();
        map.put("userA", userA);
        map.put("userB", userB);

        model.addAttribute("user", userA);
        model.addAttribute("users", list);
        model.addAttribute("userMap", map);

        return "basic/variable";
    }

    @Data
    @AllArgsConstructor
    static class User {
        private String username;
        private int age;
    }

}
```

##### variable.html

* `inflearn/SpringMVC2/thymeleaf-basic/thymeleaf-basic/src/main/resources/templates/basic/variable.html`

#### SpringEL 다양한 표현식 사용

* Object
    * `user.username`: 프로퍼티 접근 (`user.getUsername()`)
    * `user['username']`: 위와 같음(`user.getUsername()`)
    * `user.getUsername()`: user의 `getUsername()`직접 호출
* List
    * `user[0].username`: List에서 첫번째 회원을 찾고 프로퍼티 접근 (`list.get(0).getUsername()`)
    * `users[0]['username`]: 위와 같음
    * `users[0].getUsername()`: 직접 호출
* Map
    * `userMap['userA'].username`: 프로퍼티 접근 (`map.get("userA").getUsername()`)
    * `userMap['userA']['username]`: 위와 같음
    * `userMap['userA'].getUsername()`: 직접 호출

#### 지역 변수 선언

* `th:with`를 이용해 지역변수 선언 가능
* 지역변수는 선언한 테그 안에서만 사용 가능

```html

<div th:with="first=${users[0]}">
    <p>처음 사람의 이름은 <span th:text="${first.username}"></span></p>
</div>
```

### 1-5. 기본 객체들

* `${#request}`
* `${#response}`
* `${#session}`: 세션 접근
    * 예) `${session.sessionData}`
* `${#servletContext}`
* `${#locale}`
* `${#param}`
    * `#request`는 `HttpServletRequest`객체가 그대로 제공
    * 접근법이 불편함(`request.getParameter("data")`)
    * 이점을 해결하기 위한 객체(`${#param.paramData}`)
* `@`: 스프링 빈 접근
    * 예)`${@!helloBean.hello('Spring!')}`

> 세션은 이후 배울 예정    
> `Facade`패턴...???

##### BasicController.java (추가)

```java
package hello.thymeleafbasic.basic;

import lombok.AllArgsConstructor;
import lombok.Data;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

import javax.servlet.http.HttpSession;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Controller
@RequestMapping("/basic")
public class BasicController {

    // ...

    @GetMapping("/basic-objects")
    public String basicObjects(HttpSession session) {
        session.setAttribute("sessionData", "Hello Session");
        return "basic/basic-objects";
    }

    @Component("helloBean")
    static class HelloBean {
        public String hello(String data) {
            return "Hello " + data;
        }
    }
}
```

##### basic-objects.html

* `inflearn/SpringMVC2/thymeleaf-basic/thymeleaf-basic/src/main/resources/templates/basic/basic-objects.html`

```html
<!DOCTYPE html>
<html xmlns:th="http://www.thymeleaf.org">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<h1>식 기본 객체 (Expression Basic Objects)</h1>
<ul>
    <li>request = <span th:text="${#request}"></span></li>
    <li>response = <span th:text="${#response}"></span></li>
    <li>session = <span th:text="${#session}"></span></li>
    <li>servletContext = <span th:text="${#servletContext}"></span></li>
    <li>locale = <span th:text="${#locale}"></span></li>
</ul>
<h1>편의 객체</h1>
<ul>
    <li>Request Parameter = <span th:text="${param.paramData}"></span></li>
    <li>session = <span th:text="${session.sessionData}"></span></li>
    <li>spring bean = <span th:text="${@helloBean.hello('Spring!')}"></span></li>
</ul>
</body>
</html>
```

### 1-6. 유틸리티 객체와 날짜

#### 타임리프 유틸리티 객체들

* `#message`: 메시지, 국제화 처리
* `#uris`: URI 이스케이프 지원
* `#dates`: `java.util.Date`서식 지원
* `#calendars`: `java.util.Calendar`서식 지원
* `#temporals`: 자바8 날짜 서식 지원
* `#numbers`: 숫자 서식 지원
* `#strings`: 문자 관련 편의 기능
* `#objects`: 객체 관련 기능 제공
* `#bools`: boolean관련 기능 제공
* `#arrays`: 배열 관련 기능 제공
* `#lists`, `#sets`, `#maps`: 컬렉션 관련 기능 제공
* `#ids`: 아이디 처리 관련 기능 제공(뒤에서 설명)
* [타임리프 유틸리티 객체](https://www.thymeleaf.org/doc/tutorials/3.0/usingthymeleaf.html#expression-utility-objects)
  , [유틸리티 객체 예시](https://www.thymeleaf.org/doc/tutorials/3.0/usingthymeleaf.html#appendix-b-expression-utility-objects)

#### 자바8 날짜

* 타임리프에서 자바8 날짜인 `LocalDate`, `LocalDateTime`, `Instant`를 사용하려면 추가 라이브러 필요
* 스프링 부트 타임리프를 사용하면 해당 라이브러리(`thymeleaf-extras-java8time`) 자동 추가
* `#temporals`: 자바8 날짜용 유틸리티 객체
    * `<span th:text="${#temporals.format(localDateTime, 'yyyy-MM-dd HH:mm:ss`)}"></span>`

##### BasicController.java (추가)

```java
package hello.thymeleafbasic.basic;

import lombok.AllArgsConstructor;
import lombok.Data;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

import javax.servlet.http.HttpSession;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Controller
@RequestMapping("/basic")
public class BasicController {

    // ...

    @GetMapping("/date")
    public String date(Model model) {
        model.addAttribute("localDateTime", LocalDateTime.now());
        return "basic/date";
    }
}
```

##### date.html

* `inflearn/SpringMVC2/thymeleaf-basic/thymeleaf-basic/src/main/resources/templates/basic/date.html`

```html
<!DOCTYPE html>
<html xmlns:th="http://www.thymeleaf.org">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<h1>LocalDateTime</h1>
<ul>
    <li>default = <span th:text="${localDateTime}"></span></li>
    <li>yyyy-MM-dd HH:mm:ss = <span th:text="${#temporals.format(localDateTime, 'yyyy-MM-dd HH:mm:ss')}"></span></li>
</ul>
<h1>LocalDateTime - Utils</h1>
<ul>
    <li>${#temporals.day(localDateTime)} = <span th:text="${#temporals.day(localDateTime)}"></span></li>
    <li>${#temporals.month(localDateTime)} = <span th:text="${#temporals.month(localDateTime)}"></span></li>
    <li>${#temporals.monthName(localDateTime)} = <span th:text="${#temporals.monthName(localDateTime)}"></span></li>
    <li>${#temporals.monthNameShort(localDateTime)} = <span
            th:text="${#temporals.monthNameShort(localDateTime)}"></span></li>
    <li>${#temporals.year(localDateTime)} = <span th:text="${#temporals.year(localDateTime)}"></span></li>
    <li>${#temporals.dayOfWeek(localDateTime)} = <span th:text="${#temporals.dayOfWeek(localDateTime)}"></span></li>
    <li>${#temporals.dayOfWeekName(localDateTime)} = <span th:text="${#temporals.dayOfWeekName(localDateTime)}"></span>
    </li>
    <li>${#temporals.dayOfWeekNameShort(localDateTime)} = <span
            th:text="${#temporals.dayOfWeekNameShort(localDateTime)}"></span></li>
    <li>${#temporals.hour(localDateTime)} = <span th:text="${#temporals.hour(localDateTime)}"></span></li>
    <li>${#temporals.minute(localDateTime)} = <span th:text="${#temporals.minute(localDateTime)}"></span></li>
    <li>${#temporals.second(localDateTime)} = <span th:text="${#temporals.second(localDateTime)}"></span></li>
    <li>${#temporals.nanosecond(localDateTime)} = <span th:text="${#temporals.nanosecond(localDateTime)}"></span></li>
</ul>
</body>
</html>
```

### 1-7. URL 링크

##### BasicController.java (추가)

```java
package hello.thymeleafbasic.basic;

import lombok.AllArgsConstructor;
import lombok.Data;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

import javax.servlet.http.HttpSession;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Controller
@RequestMapping("/basic")
public class BasicController {

    // ...

    @GetMapping("/link")
    public String link(Model model) {
        model.addAttribute("param1", "data1");
        model.addAttribute("param2", "data2");
        return "basic/link";
    }
}
```

##### link.html

* `inflearn/SpringMVC2/thymeleaf-basic/thymeleaf-basic/src/main/resources/templates/basic/link.html`

```html
<!DOCTYPE html>
<html xmlns:th="http://www.thymeleaf.org">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<h1>URL 링크</h1>
<ul>
    <li><a th:href="@{/hello}">basic url</a></li>
    <li><a th:href="@{/hello(param1=${param1}, param2=${param2})}">hello query param</a></li>
    <li><a th:href="@{/hello/{param1}/{param2}(param1=${param1}, param2=${param2})}">path variable</a></li>
    <li><a th:href="@{/hello/{param1}(param1=${param1}, param2=${param2})}">path variable + query parameter</a></li>
</ul>
</body>
</html>
```

#### URL 정리

* 단순한 URL
    * `@{/hello}` -> `/hello`
* 쿼리 파라미터
    * `@{/hello(param1=${param1}, param2=${param2})}` -> `/hello?param1=data1&param2=data2`
    * `()`에 있는 부분은 쿼리 파라미터로 처리된다.
* 경로 변수
    * `@{/hello/{param1}/{param2}(param1=${param1}, param2=${param2})}` -> `/hello/data1/data2`
    * URL 경로상에 변수가 있으면 `()`부분은 경로 변수로 처리된다.
* 경로 변수 + 쿼리 파라미터
    * `@{/hello/{param1}(param1=${param1}, param2=${param2})}` -> `/hello/data1?param2=data2`
    * 경로 변수와 쿼리 파라미터를 함께 사용할 수 있다.
* 절대 경로: `/hello`
* 상대 경로: `hello`