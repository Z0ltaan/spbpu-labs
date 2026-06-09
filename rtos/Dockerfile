FROM huecker.io/arlaneenalra/watcom-docker:2026-06-01-Build AS build-stage

RUN apk add cmake

WORKDIR /app

COPY . .

RUN cmake --preset dos-16bit && \
    cmake --build build -j8

FROM scratch AS export-stage

COPY --from=build-stage /app/build /
