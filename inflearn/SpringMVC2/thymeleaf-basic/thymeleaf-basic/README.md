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