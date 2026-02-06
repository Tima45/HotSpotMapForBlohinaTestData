#include "qcpitemtextoutline.h"


QCPItemTextOutline::QCPItemTextOutline(QCustomPlot *parentPlot) : QCPItemText(parentPlot)
{
    // Настройки по умолчанию для контура
    mOutlinePen = QPen(Qt::black, 2); // Белый контур толщиной 1 пиксель
}

// Методы для настройки контура
void QCPItemTextOutline::setOutlinePen(const QPen &pen) { mOutlinePen = pen; }
QPen QCPItemTextOutline::outlinePen() const { return mOutlinePen; }



// Переопределяем метод отрисовки
void QCPItemTextOutline::draw(QCPPainter *painter)
{
    // 1. Получаем позицию и трансформации (логика скопирована из QCPItemText)
    QPointF pos(position->pixelPosition());
    QTransform transform = painter->transform();
    transform.translate(pos.x(), pos.y());
    if (!qFuzzyIsNull(mRotation))
        transform.rotate(mRotation);

    // 2. Подготовка шрифтов и прямоугольников
    painter->setFont(mainFont());
    QRect textRect = painter->fontMetrics().boundingRect(0, 0, 0, 0, Qt::TextDontClip|mTextAlignment, mText);
    QRect textBoxRect = textRect.adjusted(-mPadding.left(), -mPadding.top(), mPadding.right(), mPadding.bottom());
    QPointF textPos = getTextDrawPoint(QPointF(0, 0), textBoxRect, mPositionAlignment);
    textRect.moveTopLeft(textPos.toPoint() + QPoint(mPadding.left(), mPadding.top()));
    textBoxRect.moveTopLeft(textPos.toPoint());

    double clipPad = mainPen().widthF();
    QRect boundingRect = textBoxRect.adjusted(-clipPad, -clipPad, clipPad, clipPad);

    // 3. Проверка видимости
    if (transform.mapRect(boundingRect).intersects(painter->transform().mapRect(clipRect())))
    {
        painter->setTransform(transform);

        // Рисуем фон (прямоугольник), если задан brush/pen у самого айтема
        if ((mainBrush().style() != Qt::NoBrush && mainBrush().color().alpha() != 0) ||
            (mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0))
        {
            painter->setPen(mainPen());
            painter->setBrush(mainBrush());
            painter->drawRect(textBoxRect);
        }

        // 4. РИСУЕМ КОНТУР (АУТЛАЙН)
        if (mOutlinePen.style() != Qt::NoPen)
        {
            painter->setBrush(Qt::NoBrush);
            painter->setPen(mOutlinePen);

            // Рисуем текст 8 раз со смещением (эффект гало)
            // Чем толще mOutlinePen, тем больше должен быть шаг, но для текста обычно достаточно 1-2px
            int steps = 8;
            double radius = qMax(1.0, mOutlinePen.widthF());

            for (int i = 0; i < steps; ++i)
            {
                double angle = 2 * M_PI * i / steps;
                double dx = radius * qCos(angle);
                double dy = radius * qSin(angle);
                painter->drawText(textRect.translated(dx, dy), Qt::TextDontClip|mTextAlignment, mText);
            }
        }

        // 5. Рисуем основной текст поверх контура
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(mainColor()));
        painter->drawText(textRect, Qt::TextDontClip|mTextAlignment, mText);
    }
}
