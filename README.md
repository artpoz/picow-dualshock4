# picow-dualshock4

```sh
sh build.sh
```

The .uf2 file will be build/src/picow_ds4.uf2

## LINKS

<https://github.com/usedbytes/picow_ds4>
<https://github.com/bluekitchen/btstack/blob/master/example/gap_inquiry.c>

## memo

```sh
# def
buttons: 0000, l: 128,128, r: 128,128, l2,r2: 0,0 hat: 8
buttons: 0000, l: 左スティック(左が0 右が255),左スティック(上が0 下が255), r: 右スティック(左が0 右が255),右スティック(上が0 下が255), l2,r2: L2ボタン(押し込んだら255),R2ボタン(押し込んだら255)  hat: 8

```

<table>
<caption>buttons</caption>
<thead>
<th colspan="2">0</th><th colspan="2">0</th><th colspan="2">0</th><th colspan="2">0</th>
</thead>
<tbody>
<tr>
<th>key</th>
<th>num</th>
<th>key</th>
<th>num</th>
<th>key</th>
<th>num</th>
<th>key</th>
<th>num</th>
</tr>
<tr>
<td>□</td>
<td>1</td>
<td>home</td>
<td>1</td>
<td>share</td>
<td>1</td>
<td>L1</td>
<td>1</td>
</tr>
<tr>
<td>×</td>
<td>2</td>
<td>tach pad</td>
<td>2</td>
<td>options</td>
<td>2</td>
<td>R1</td>
<td>2</td>
</tr>
<tr>
<td>○</td>
<td>4</td>
<td></td>
<td></td>
<td>L stick push</td>
<td>4</td>
<td>L2</td>
<td>4</td>
</tr>
<tr>
<td>△</td>
<td>8</td>
<td></td>
<td></td>
<td>R stick push</td>
<td>8</td>
<td>R2</td>
<td>8</td>
</tr>
</tbody>
</table>

<table>
<caption>hat</caption>
<thead><th>key</th><th>num</th></thead>
<tbody>
<tr><td>↑</td><td>0</td></tr>
<tr><td>→</td><td>2</td></tr>
<tr><td>↓</td><td>4</td></tr>
<tr><td>←</td><td>6</td></tr>
</tbody>
</table>
