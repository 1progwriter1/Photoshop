# Photoshop

Графический редактор, реализованный на базе SFML

## Возможности

### Рисование кистью

Возможность выбора цвета. При реализации использована интерполяция.

### Геометрические фигуры

**Доступны**

- прямоугольник
- эллипс
- линия

### Фильтры

**Барельеф**

<table>
  <tr>
    <td align="center">
      <img src="readme_img/barelief_before.png" width="200">
      <br>
      <small>До</small>
    </td>
    <td align="center">
      <img src="readme_img/barelief_after.png" width="200">
      <br>
      <small>После</small>
    </td>
  </tr>
</table>

**Нерезкое маскирование**

<table>
  <tr>
    <td align="center">
      <img src="readme_img/unsharp_mask_before.png" width="200">
      <br>
      <small>До</small>
    </td>
    <td align="center">
      <img src="readme_img/unsharp_mask_after.png" width="200">
      <br>
      <small>После</small>
    </td>
  </tr>
</table>

**Обычное размытие**

<table>
  <tr>
    <td align="center">
      <img src="readme_img/blur_before.png" width="200">
      <br>
      <small>До</small>
    </td>
    <td align="center">
      <img src="readme_img/blur_after.png" width="200">
      <br>
      <small>После</small>
    </td>
  </tr>
</table>

**Негатив**

<table>
  <tr>
    <td align="center">
      <img src="readme_img/negative_before.png" width="200">
      <br>
      <small>До</small>
    </td>
    <td align="center">
      <img src="readme_img/negative_after.png" width="200">
      <br>
      <small>После</small>
    </td>
  </tr>
</table>

**Яркость**

Для фильтра яркости доступна панель для выбора уровня яркости для всех вертикальных зон индивидуально.

<table>
  <tr>
    <td align="center">
      <img src="readme_img/brightness.png" width="200">
      <br>
      <small>Изображение</small>
    </td>
    <td align="center">
      <img src="readme_img/panel.png" width="200">
      <br>
      <small>Панель</small>
    </td>
  </tr>
</table>

**Размытие Гаусса**

<table>
  <tr>
    <td align="center">
      <img src="readme_img/gauss_blur_before.png" width="200">
      <br>
      <small>До</small>
    </td>
    <td align="center">
      <img src="readme_img/gauss_blur_after.png" width="200">
      <br>
      <small>После</small>
    </td>
  </tr>
</table>

## Плагины

Добавлять новые инструменты и фильтры можно при помощи динамически загружаемых плагинов, которые должны соответствовать [api](api/).
